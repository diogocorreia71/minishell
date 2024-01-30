/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:36:27 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/30 17:02:35 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_hereDoc_input(char *input, char *delimiter)
{
	if (input == NULL)
	{
		//DIzer a linha em que parou do hereDoc, ctrl+d dentro do hereDoc(ver mais tarde)
		printf("warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(delimiter, input) == 0)
		return (1);
	return (0);
}

void	init_hereDoc_handler(void)
{
	struct sigaction	sig;
	
	sig.sa_flags = 0;
	sigemptyset(&(sig.sa_mask));
	sig.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
}

void	init_hereDoc(t_heredoc *here_doc, t_env *env)
{
	int		hereDoc_fd;
	pid_t	hereDoc_pid;	

	hereDoc_fd = 0;
	here_doc->delimiter = remove_quotes(here_doc->delimiter);
	hereDoc_pid = fork();
	if (hereDoc_pid == 0)
	{
		hereDoc_fd = open("hereDoc", here_doc->open_flags, 0644);
		if (hereDoc_fd == -1)
		{
			ft_fprintf(STDERR_FILENO, "Error starting hereDoc fd\n");
			exit(1);
		}
		init_hereDoc_handler();
		run_hereDoc(here_doc, env, hereDoc_fd);
	}
	waitpid(-1, NULL, 0);
	init_signal_handler(main_signal_handler);
}

void	executer_cmd(t_minishell *shell, t_generic *cmd)
{
	if (cmd->type == EXEC)
		run_exec(shell, (t_exec *)cmd);
	else if (cmd->type == REDIR)
		run_redir(shell, (t_redir *)cmd);
	else if (cmd->type == PIPE)
		run_pipe(shell, (t_pipe *)cmd);
	else if (cmd->type == HERE_DOC)
	{
		run_redir(shell, (t_redir *)((t_heredoc *)cmd)->heredoc_redir);
		unlink("hereDoc");
	}
}
