/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:12:07 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/29 11:40:53 by rumachad         ###   ########.fr       */
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
}

void	free_prev_node(t_cmd **arg)
{
	t_cmd	*tmp;

	tmp = *arg;
	*arg = (*arg)->next;
	free(tmp->token);
	free(tmp);
}

void	free_tokens(t_cmd *tokens)
{
	while (tokens != NULL)
		free_prev_node(&tokens);
}

void	free_all_child(t_minishell *shell, t_pipe *info)
{
	ft_free_dp((void **)shell->cmd_split);
	ft_free_dp((void **)info->fd);
	ft_free_dp((void **)shell->env_array);
	free(info->pipe_pid);
	free_env(shell->env);
	free(shell->rl_str);
}

