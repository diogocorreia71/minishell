/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:36:27 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/24 16:26:15 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_hereDoc(t_generic *struct_pointer, char *delimiter)
{
	t_redir	*here_doc;
	char	*input;
	int		here_doc_fd;

	here_doc = (t_redir *)struct_pointer;
	here_doc->type = HERE_DOC;
	here_doc_fd = open("hereDoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (here_doc_fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "Error starting hereDoc fd\n");
		return ;
	}
	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(delimiter, input) == 0)
			break ;
		ft_fprintf(here_doc_fd, "%s\n", input);
		free(input);
	}
	close(here_doc_fd);
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
		run_redir(shell, (t_redir *)cmd);
		unlink("hereDoc");
	}
}
