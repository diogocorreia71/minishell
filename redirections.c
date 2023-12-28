/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:38:34 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/28 13:25:15 by rumachad         ###   ########.fr       */
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
	while (args->type != redir)
		args = args->next;
	args = args->next;
	return (args->token);
}

void	free_redir(t_cmd *args)
{
	while (args->next->type != redir)
		args = args->next;
	args->next = NULL;
	args = args->next;
	while (args != NULL)
		free_prev_node(&args);
}

int	handle_redir(t_minishell *shell, t_cmd *args)
{
	t_rdr	rr; //trocar nome
	char	*redir_path;
	
	rr.nbr_redir = count_redir(args);
	if (rr.nbr_redir == 0)
		return (0);
	//Talvez dar fork para as redirections
	redir_path = redir_pathname(args);
	rr.file_fd = open(redir_path, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (rr.file_fd == -1)
		return (0);
	rr.orig_fd = dup(STDOUT_FILENO);
	dup2(rr.file_fd, STDOUT_FILENO);
	close(rr.file_fd);
	free_redir(args);
	shell->rr = rr;
	return (1);
}
