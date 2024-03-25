/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:52:09 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 19:01:33 by rumachad         ###   ########.fr       */
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
		{
			check_close(close(heredoc_fd));
			return (-1);
		}
		else if (ft_strcmp(here_doc->delimiter, input) == 0)
		{
			free(input);
			break ;
		}
		if (here_doc->expansion == YES)
			input = expand_token(env, input, handle_heredoc_ds);
		ft_fprintf(heredoc_fd, "%s\n", input);
		free(input);
	}
	check_close(close(heredoc_fd));
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
		ft_execve(shell);
}

void	run_redir(t_minishell *shell, t_redir *cmd)
{
	int	file_fd;
	int	orig_fd;

	orig_fd = check_dup(dup(cmd->redir_fd));
	file_fd = open(cmd->filename, cmd->open_flags, 0644);
	if (file_fd == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"%s: No such file or directory\n", cmd->filename);
		g_exit_status = 1;
		check_close(close(orig_fd));
		return ;
	}
	check_dup(dup2(file_fd, cmd->redir_fd));
	check_close(close(file_fd));
	executer_cmd(shell, cmd->last_pointer);
	check_dup(dup2(orig_fd, cmd->redir_fd));
	check_close(close(orig_fd));
}

void	run_pipe(t_minishell *shell, t_gen *cmd, int pipe_fd[2], int fd)
{
	shell->in_pipe = YES;
	init_signals(SIGIGNORE);
	check_dup(dup2(pipe_fd[fd], fd));
	close_fd(pipe_fd);
	executer_cmd(shell, cmd);
	free_child(shell->ast_head, shell->env);
	exit(g_exit_status);
}

void	run_pipeline(t_minishell *shell, t_pipe *cmd)
{
	t_pipeline	pipeline;
	int			status;

	status = 0;
	if (check_fd(pipe(pipeline.pipe_fd), "pipe") == -1)
		return ;
	pipeline.pipe_pid_left = fork();
	if (check_fork(pipeline.pipe_pid_left) < 0)
		return ;
	if (pipeline.pipe_pid_left == 0)
		run_pipe(shell, cmd->left, pipeline.pipe_fd, STDOUT_FILENO);
	pipeline.pipe_pid_right = fork();
	if (check_fork(pipeline.pipe_pid_right) < 0)
		return ;
	if (pipeline.pipe_pid_right == 0)
		run_pipe(shell, cmd->right, pipeline.pipe_fd, STDIN_FILENO);
	close_fd(pipeline.pipe_fd);
	check_wait(waitpid(pipeline.pipe_pid_left, &status, 0));
	check_wait(waitpid(pipeline.pipe_pid_right, &status, 0));
	g_exit_status = WEXITSTATUS(status);
}
