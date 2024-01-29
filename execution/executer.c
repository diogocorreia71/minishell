/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:36:27 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/28 22:08:24 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_hereDoc(t_generic *struct_pointer, t_env *env)
{
	t_heredoc	*here_doc;
	int			here_doc_fd;
	char		*input;

	here_doc = (t_heredoc *)struct_pointer;
	here_doc->delimiter = remove_quotes(here_doc->delimiter);
	here_doc_fd = open("hereDoc", here_doc->open_flags, 0644);
	if (here_doc_fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "Error starting hereDoc fd\n");
		return ;
	}
	while (1)
	{
		input = readline("> ");
		if (ft_strcmp(here_doc->delimiter, input) == 0)
			break ;
		if (here_doc->expansion == YES)
			input = expand_token(env, input);
		ft_fprintf(here_doc_fd, "%s\n", input);
		free(input);
	}
	free(input);
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
		run_redir(shell, (t_redir *)((t_heredoc *)cmd)->heredoc_redir);
		unlink("hereDoc");
	}
}
