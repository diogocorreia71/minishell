/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:35 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/29 13:00:20 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unset(char *cmd)
{
	int	i;

	i = 0;
	if (ft_isdigit(cmd[i]) == 1)
		return (1);
	while (cmd[i])
	{
		if (ft_isalnum(cmd[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	unset_syntax(char *cmd)
{
	if (check_unset(cmd) == 1)
	{
		ft_fprintf(2, "unset: `%s': not a valid identifier\n", cmd);
		g_exit_status = 1;
		return (1);
	}
	return (0);
}

void	del_var(t_env *env, t_env *tmp)
{	
	while (env->next->var != tmp->var)
		env = env->next;
	env->next =env->next->next;
	free(tmp->var);
	free(tmp->var_value);
	free(tmp);
}

void	unset(t_env *env, char **cmd_split)
{
	t_env	*tmp;
	int		i;
	
	i = -1;
	while (cmd_split[++i])
	{
		if (unset_syntax(cmd_split[i]) == 1)
			continue;
		tmp = get_env_node(env, cmd_split[i]);
		if (tmp == NULL)
			continue;
		del_var(env, tmp);
	}
}
