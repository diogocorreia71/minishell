/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:34:11 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 15:50:35 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gen	*exec_constructor(void)
{
	t_exec	*exec_cmd;

	exec_cmd = (t_exec *)ft_calloc(sizeof(t_exec), 1);
	exec_cmd->type = EXEC;
	exec_cmd->argv = NULL;
	return ((t_gen *)exec_cmd);
}

t_gen	*redir_constructor(t_gen *cmd, int fd, int flags, char *file)
{
	t_redir	*redir_cmd;
	t_gen	*last_redir;
	t_gen	*tmp;

	redir_cmd = (t_redir *)ft_calloc(sizeof(t_redir), 1);
	redir_cmd->type = REDIR;
	redir_cmd->redir_fd = fd;
	redir_cmd->open_flags = flags;
	redir_cmd->filename = ft_strdup(file);
	if (cmd->type == EXEC)
		redir_cmd->last_pointer = cmd;
	else
	{
		tmp = cmd;
		while (tmp->type != EXEC)
		{
			last_redir = tmp;
			tmp = ((t_redir *)tmp)->last_pointer;
		}
		((t_redir *)last_redir)->last_pointer = (t_gen *)redir_cmd;
		redir_cmd->last_pointer = tmp;
		return (cmd);
	}
	return ((t_gen *)redir_cmd);
}

t_gen	*pipe_constructor(t_gen *left, t_gen *right)
{
	t_pipe	*pipe_cmd;

	pipe_cmd = (t_pipe *)ft_calloc(sizeof(t_pipe), 1);
	pipe_cmd->type = PIPE;
	pipe_cmd->left = left;
	pipe_cmd->right = right;
	return ((t_gen *)pipe_cmd);
}

t_gen	*heredoc_constructor(t_gen *cmd, char *delimiter, int flags)
{
	t_heredoc	*heredoc_cmd;

	heredoc_cmd = (t_heredoc *)ft_calloc(sizeof(t_heredoc), 1);
	heredoc_cmd->type = HERE_DOC;
	heredoc_cmd->delimiter = delimiter;
	heredoc_cmd->open_flags = flags;
	heredoc_cmd->heredoc_redir = cmd;
	heredoc_cmd->expansion = YES;
	if (token_has_quotes(delimiter) == YES)
		heredoc_cmd->expansion = NO;
	return ((t_gen *)heredoc_cmd);
}
