/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:35 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/21 17:32:39 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Trocar nome
int	check_unset(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_isalpha(cmd[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	unset_syntax(char **cmd_split)
{
	int	i;
	int	flag;

	if (cmd_split[1][0] == '\0')
		return (1);
	i = 1;
	flag = 0;
	while (cmd_split[i])
	{
		if (check_unset(cmd_split[i]) == 1)
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(cmd_split[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			flag = 1;
		}
		i++;
	}
	if (flag == 1)
		return (1);
	else
		return (0);
}

void	unset(t_env *env, char **cmd_split)
{
	t_env	*tmp;

	if (cmd_split[1] == NULL)
		return ;
	if (unset_syntax(cmd_split) == 1)
		return ;
	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strcmp(cmd_split[1], tmp->var) == 0)
			break ;
		tmp = tmp->next;
	}
	while (env->next->var != tmp->var)
		env = env->next;
	env->next =env->next->next;
	free(tmp->var);
	free(tmp->var_value);
	free(tmp);
}
