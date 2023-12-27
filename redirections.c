/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:38:34 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 16:38:56 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_redir(t_cmd *args)
{
	int		nbr_redir;

	nbr_redir = 0;
	while (args != NULL)
	{
		if (args->type == redir)
			nbr_redir++;
		args = args->next;
	}
	return (nbr_redir);
}

char	*redir_pathname(t_cmd *args)
{
	t_cmd	*tmp;

	tmp = args;
	while (tmp->type != redir)
		tmp = tmp->next;
	tmp = tmp->next;
	return (tmp->token);
}

void	free_redir(t_cmd *args)
{
	t_cmd	*tmp;
	t_cmd	*tmp2;
	
	while (args->next->type != redir)
		args = args->next;
	tmp2 = args;
	args->next = NULL;
	args = args->next;
	while (args != NULL)
	{
		tmp = args;
		args = args->next;
		free(tmp->token);
		free(tmp);
	}
}

int	handle_redir(t_minishell *shell, t_cmd *args)
{
	t_rdr	rr; //trocar nome
	char	*redir_path;
	
	rr.nbr_redir = count_redir(args);
	if (rr.nbr_redir == 0)
		return (0);
	redir_path = redir_pathname(args);
	rr.file_fd = open(redir_path, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	rr.orig_fd = dup(STDOUT_FILENO);
	dup2(rr.file_fd, STDOUT_FILENO);
	close(rr.file_fd);
	free_redir(args);
	shell->rr = rr;
	return (1);
}
