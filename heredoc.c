/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:39:25 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/01 15:39:53 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_heredoc_input(char *input, char *delimiter)
{
	if (input == NULL)
	{
		ft_fprintf(STDERR_FILENO, "warning: here-document ");
		ft_fprintf(STDERR_FILENO, "delimited by end-of-file (wanted `%s')\n"
			, delimiter);
		exit(0);
	}
	if (ft_strcmp(delimiter, input) == 0)
		return (1);
	return (0);
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
	if (heredoc_pid == 0)
	{
		heredoc_fd = open("hereDoc", here_doc->open_flags, 0644);
		if (heredoc_fd == -1)
		{
			ft_fprintf(STDERR_FILENO, "Error starting hereDoc fd\n");
			exit(1);
		}
		init_signals(SIGHERE);
		run_heredoc(here_doc, env, heredoc_fd);
		free_heredoc(head, env, (t_generic *)here_doc);
		exit(0);
	}
	init_signals(IGNORE);
	waitpid(heredoc_pid, &status, 0);
	heredoc_exit_status(here_doc, status);
	init_signals(SIGMAIN);
}
