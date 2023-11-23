/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:30:05 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/23 17:20:05 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_env *env, char *cmd);

void	print_error(char *var, int error)
{
	if (error == 1)
		printf("export: `%s': not a valid identifier\n", var);
	else
		return ;
}

int	check_export(char *var)
{
	char	*tmp;

	tmp = var;
	//Melhorar o check dos erros do export
	if (ft_strchr(var, '=') == NULL)
	{
		print_error(var, 1);
		return (1);
	}
	while (*tmp != '=')
	{
		if (ft_isdigit(*tmp) == 1)
			break ;
		tmp++;
	}
	if (*tmp == '=')
		return (0);
	print_error(var, 1);
	return (1);
}

void	export(t_env *env, char *var)
{
	char	*tmp;
	int		k;
	int		i;

	if (check_export(var) == 1)
		return ;
	i = 0;
	while (var[i] != '=' && var[i])
		i++;
	tmp = (char *)malloc(sizeof(char) * (i + 1));
	if (tmp == NULL)
		return ;
	k = -1;
	while (++k < i)
		tmp[k] = var[k];
	tmp[k] = '\0';
	//SEG_FAULT Quando o argmento: a =0
	if (get_env(env, tmp))
		unset(env, tmp);
	env = env_last(env);
	env->next = create_node(tmp, var);
	free(tmp);
}
