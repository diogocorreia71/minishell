/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:52:09 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/22 18:36:37 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_exec(t_minishell *shell, t_exec *cmd)
{
	shell->cmd_split = ft_split(cmd->argv, ' ');
	free(cmd->argv);
	if (is_builtin(shell->cmd_split[0]) == YES)
		builtin_cmd(shell, cmd);
	else
	{
		if (non_builtin(shell) == -1)
			return ;
		ft_execve(shell);
	}
	free(cmd);
}

void	run_redir(t_minishell *shell, t_redir *cmd)
{
	int	file_fd;
	int	orig_fd;
	
	orig_fd = dup(cmd->redir_fd);
	file_fd = open(cmd->filename, cmd->open_flags, 0644);
	if (cmd->redir_fd == 1)
	{
		dup2(file_fd, cmd->redir_fd);
		close(file_fd);
		executer_cmd(shell, cmd->cmd);
	}
	dup2(orig_fd, cmd->redir_fd);
	close(orig_fd);
	free(cmd->filename);
	free(cmd);
}

void	run_pipe(t_minishell *shell, t_pipe *cmd)
{
	int	pipe_fd[2];
	int	pipe_pid;
	
	if (pipe(pipe_fd) == -1)
	{
		ft_fprintf(STDERR_FILENO, "Pipe creation error\n");
		return ;
	}
	pipe_pid = fork();
	if (pipe_pid == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close_fd(pipe_fd);
		executer_cmd(shell, cmd->left);
		exit(0);
	}
	pipe_pid = fork();
	if (pipe_pid == 0)
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		close_fd(pipe_fd);
		executer_cmd(shell, cmd->right);
		exit(0);
	}
	close_fd(pipe_fd);
	waitpid(pipe_pid, NULL, 0);
}
