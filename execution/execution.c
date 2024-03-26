/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 10:13:43 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/26 10:45:22 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executer_cmd(t_minishell *shell, t_gen *cmd)
{
	if (cmd->type == EXEC)
		run_exec(shell, (t_exec *)cmd);
	else if (cmd->type == REDIR)
		run_redir(shell, (t_redir *)cmd);
	else if (cmd->type == PIPE)
		run_pipeline(shell, (t_pipe *)cmd);
	else if (cmd->type == HERE_DOC)
	{
		if (((t_heredoc *)cmd)->heredoc_redir != NULL)
			executer_cmd(shell, ((t_heredoc *)cmd)->heredoc_redir);
		unlink("hereDoc");
	}
}

void	wait_proccess(int pipe_pid)
{
	int	status;

	status = 0;
	if (waitpid(pipe_pid, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = WTERMSIG(status) + 128;
	}
	while (wait(0) != -1)
		;
}
