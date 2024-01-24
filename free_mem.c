/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:12:07 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/23 23:38:41 by rui              ###   ########.fr       */
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

void	free_tokens(t_lst_tokens **args)
{
	t_lst_tokens	*tmp;
	
	while (*args != NULL)
	{
		tmp = *args;
		*args = (*args)->next;
		free(tmp->token);
		free(tmp);
	}
}

void	free_tree(t_generic *cmd)
{
	if (cmd->type == EXEC)
	{
		free(((t_exec *)cmd)->argv);
		free(((t_exec *)cmd));
	}
	else if (cmd->type == REDIR)
	{
		free_tree(((t_redir *)cmd)->cmd);
		free(((t_redir *)cmd)->filename);
		free(((t_redir *)cmd));
	}
	else if (cmd->type == PIPE)
	{
		free_tree(((t_pipe *)cmd)->left);
		free_tree(((t_pipe *)cmd)->right);
		free(cmd);
	}
}

void	free_child(t_minishell *shell, t_pipe *cmd)
{
	free_tree((t_generic *)cmd);
	free_env(shell->env);
}
