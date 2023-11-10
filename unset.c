/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:35 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/10 12:29:45 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_env *env, char *cmd)
{
	t_env	*tmp;

	if (!get_env(env, cmd))
		return ;
	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strcmp(cmd, tmp->var) == 0)
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
