/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:12:07 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/22 16:26:09 by rumachad         ###   ########.fr       */
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

void	execve_clean(t_minishell *shell)
{
	free(shell->path);
	ft_free_dp((void **)shell->cmd_split);
	ft_free_dp((void **)shell->env_array);
}

void	free_prev_node(t_lst_tokens **arg)
{
	t_lst_tokens	*tmp;

	tmp = *arg;
	*arg = (*arg)->next;
	free(tmp->token);
	free(tmp);
}

void	free_tokens(t_lst_tokens **tokens)
{
	while (*tokens != NULL)
		free_prev_node(tokens);
}

void	free_all_child(t_minishell *shell)
{
	ft_free_dp((void **)shell->cmd_split);
	free_env(shell->env);
}

