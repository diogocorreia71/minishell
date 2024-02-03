/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:52:09 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 03:30:45 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_heredoc(t_heredoc *here_doc, t_env *env, int heredoc_fd)
{
	char	*input;
	
	while (1)
	{
		input = readline("> ");
		if (input == NULL)
			return (-1);
		else if (ft_strcmp(here_doc->delimiter, input) == 0)
		{
			free(input);
			break ;
		}
		if (here_doc->expansion == YES)
			input = expand_token(env, input);
		ft_fprintf(heredoc_fd, "%s\n", input);
		free(input);
	}
	close(heredoc_fd);
	return (0);
}

void	run_exec(t_minishell *shell, t_exec *cmd)
{
	g_exit_status = 0;
	if (cmd->argv[0] == 0)
		return ;
	shell->cmd_split = cmd->argv;
	if (is_builtin(shell->cmd_split[0]) == YES)
		builtin_cmd(shell, cmd);
	else
	{
		if (non_builtin(shell) == -1)
			return ;
		ft_execve(shell);
	}
}

void	run_redir(t_minishell *shell, t_redir *cmd)
{
	int	file_fd;
	int	orig_fd;
	
	orig_fd = dup(cmd->redir_fd);
	file_fd = open(cmd->filename, cmd->open_flags, 0644);
	if (file_fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "%s: No such file or directory\n", cmd->filename);
		close(orig_fd);
		return ;
	}
	dup2(file_fd, cmd->redir_fd);
	close(file_fd);
	executer_cmd(shell, cmd->last_pointer);
	dup2(orig_fd, cmd->redir_fd);
	close(orig_fd);
}

void	run_pipe(t_minishell *shell, t_pipe *cmd, int pipe_fd[2], int fd)
{
	shell->in_pipe = YES;
	init_signals(SIGCHILD);
	dup2(pipe_fd[fd], fd);
	close_fd(pipe_fd);
	if (fd == 1)
		executer_cmd(shell, cmd->left);
	else
		executer_cmd(shell, cmd->right);
	free_child(shell, cmd);
	exit(g_exit_status);
}

void	run_pipeline(t_minishell *shell, t_pipe *cmd)
{
	int	pipe_fd[2];
	int	pipe_pid_right;
	int	pipe_pid_left;
	int	status;
	
	status = 0;
	if (pipe(pipe_fd) == -1)
	{
		ft_fprintf(STDERR_FILENO, "Pipe creation error\n");
		return ;
	}
	pipe_pid_left = fork();
	if (pipe_pid_left == 0)
		run_pipe(shell, cmd, pipe_fd, STDOUT_FILENO);
	pipe_pid_right = fork();
	if (pipe_pid_right == 0)
		run_pipe(shell, cmd, pipe_fd, STDIN_FILENO);
	close_fd(pipe_fd);
	init_signals(SIGIGNORE);
	waitpid(pipe_pid_left, &status, 0);
	waitpid(pipe_pid_right, &status, 0);
	g_exit_status = WEXITSTATUS(status);
}
