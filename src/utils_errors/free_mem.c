/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:12:07 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/26 18:00:28 by rumachad         ###   ########.fr       */
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
	if (cmd != NULL)
		free(cmd);
}

void	free_redir(t_redir *cmd)
{
	if (cmd->filename != NULL)
		free(cmd->filename);
	if (cmd != NULL)
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
		if (((t_heredoc *)cmd)->delimiter != NULL)
			free(((t_heredoc *)cmd)->delimiter);
		if (((t_heredoc *)cmd) != NULL)
			free(((t_heredoc *)cmd));
	}
	else if (cmd->type == PIPE)
	{
		free_tree(((t_pipe *)cmd)->left);
		free_tree(((t_pipe *)cmd)->right);
		if (cmd != NULL)
			free(cmd);
	}
}

void	free_child(t_gen *ast_head, t_env *env)
{
	free_tree(ast_head);
	free_env(env);
}
