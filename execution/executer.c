/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:36:27 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 12:20:58 by rumachad         ###   ########.fr       */
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
			run_redir(shell, (t_redir *)((t_heredoc *)cmd)->heredoc_redir);
		unlink("hereDoc");
	}
}
