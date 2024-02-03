/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:39:25 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 17:40:35 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	eof_heredoc(char *del)
{
	ft_fprintf(STDERR_FILENO, "warning: here-document ");
	ft_fprintf(STDERR_FILENO, "delimited by end-of-file (wanted `%s')\n", del);
}

void	heredoc_exit_status(t_heredoc *here_doc, int status)
{
	int	exit_status;

	exit_status = 0;
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		g_exit_status = exit_status;
	}
	else if (WIFSIGNALED(status))
	{
		exit_status = WTERMSIG(status);
		printf("\n");
		g_exit_status = 128 + exit_status;
		free_tree(here_doc->heredoc_redir);
		here_doc->heredoc_redir = NULL;
	}
}

void	init_heredoc(t_heredoc *here_doc, t_env *env, t_lst_tokens *head)
{
	int		heredoc_fd;
	pid_t	heredoc_pid;
	int		status;	

	heredoc_fd = 0;
	here_doc->delimiter = remove_quotes(here_doc->delimiter);
	heredoc_pid = fork();
	check_fork(heredoc_pid);
	if (heredoc_pid == 0)
	{
		heredoc_fd = open("hereDoc", here_doc->open_flags, 0644);
		check_fd(heredoc_fd, "hereDoc");
		init_signals(SIGHERE);
		if (run_heredoc(here_doc, env, heredoc_fd) == -1)
			eof_heredoc(here_doc->delimiter);
		free_heredoc(head, env, (t_gen *)here_doc);
		exit(0);
	}
	init_signals(SIGIGNORE);
	check_wait(waitpid(heredoc_pid, &status, 0));
	heredoc_exit_status(here_doc, status);
	init_signals(SIGMAIN);
}
