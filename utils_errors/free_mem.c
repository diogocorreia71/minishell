/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:12:07 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/22 13:03:19 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc(t_lst_tokens *head, t_env *env, t_gen *cmd)
{
	free_env(env);
	free_tokens(&head);
	free_tree((t_gen *)cmd);
}

void	free_exec(t_exec *cmd)
{
	if (cmd->argv != NULL)
		ft_free_dp((void **)cmd->argv);
	free(cmd);
}

void	free_redir(t_redir *cmd)
{
	free(cmd->filename);
	free(cmd);
}

void	free_tree(t_gen *cmd)
{
	if (cmd == NULL)
		return ;
	if (cmd->type == EXEC)
		free_exec((t_exec *)cmd);
	else if (cmd->type == REDIR)
	{
		free_tree(((t_redir *)cmd)->last_pointer);
		free_redir((t_redir *)cmd);
	}
	else if (cmd->type == HERE_DOC)
	{
		free_tree(((t_heredoc *)cmd)->heredoc_redir);
		free(((t_heredoc *)cmd)->delimiter);
		free(((t_heredoc *)cmd));
	}
	else if (cmd->type == PIPE)
	{
		free_tree(((t_pipe *)cmd)->left);
		free_tree(((t_pipe *)cmd)->right);
		free(cmd);
	}
}

void	free_child(t_minishell *shell, t_gen *cmd)
{
	free_tree(cmd);
	free_env(shell->env);
}
