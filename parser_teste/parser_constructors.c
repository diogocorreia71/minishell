/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 19:01:15 by rui               #+#    #+#             */
/*   Updated: 2024/01/21 22:20:25 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

cmd *exec_constructor(void)
{
	exec *exec_cmd;
	
	exec_cmd = (exec *)malloc(sizeof(exec) * 1);
	exec_cmd->type = EXEC;
	return ((cmd *)exec_cmd);
}

cmd *pipe_constructor(cmd *left, cmd *right)
{
	t_pipe *pipes;

	pipes = (t_pipe *)malloc(sizeof(t_pipe) * 1);
	pipes->type = pipe;
	pipes->left = left;
	pipes->right = right;
	return ((cmd *)pipes);
}

cmd *redir_constructor(cmd *exec_cmd, int fd, int open_flags, char *filename)
{
	t_redir	*redir_cmd;

	redir_cmd = (t_redir *)malloc(sizeof(t_redir) * 1);
	redir_cmd->type = redir;
    redir_cmd->cmd = exec_cmd;
	redir_cmd->fd = fd;
	redir_cmd->filename = filename;
	redir_cmd->flags = open_flags;
	return ((cmd *)redir_cmd);
}
