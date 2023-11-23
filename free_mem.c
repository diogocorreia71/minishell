/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:12:07 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/23 15:05:25 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp->var);
		free(tmp->var_value);
		free(tmp);
	}	
}

void	clean_program(t_minishell *shell)
{
	ft_free_dp((void **)shell->cmd_split);
	free(shell->rl_str);
	free(shell->cmd);
}
