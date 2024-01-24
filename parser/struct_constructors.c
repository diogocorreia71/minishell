/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:34:11 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/24 12:30:40 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_generic	*exec_constructor(void)
{
	t_exec	*exec_cmd;

	exec_cmd = (t_exec *)malloc(sizeof(t_exec) * 1);
	exec_cmd->type = EXEC;
	exec_cmd->argv = NULL;
	return ((t_generic *)exec_cmd);
}

t_generic	*redir_constructor(t_generic *cmd, int fd, int flags, char *filename)
{
	t_redir		*redir_cmd;
	t_generic	*last_redir;
	t_generic	*tmp;

	redir_cmd = (t_redir *)malloc(sizeof(t_redir) * 1);
	redir_cmd->type = REDIR;
	redir_cmd->redir_fd = fd;
	redir_cmd->open_flags = flags;
	redir_cmd->filename = ft_strdup(filename);
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
		((t_redir *)last_redir)->last_pointer = (t_generic *)redir_cmd;
		redir_cmd->last_pointer = tmp;
		return (cmd);
	}
	return ((t_generic *)redir_cmd);
}

t_generic	*pipe_constructor(t_generic *left, t_generic *right)
{
	t_pipe	*pipe_cmd;

	pipe_cmd = (t_pipe *)malloc(sizeof(t_pipe) * 1);
	pipe_cmd->type = PIPE;
	pipe_cmd->left = left;
	pipe_cmd->right = right;
	return ((t_generic *)pipe_cmd);
}
