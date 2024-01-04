/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:38:34 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/04 17:33:40 by rumachad         ###   ########.fr       */
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
	t_rdr	hredir;
	char	*redir_path;
	
	hredir.nbr_redir = count_redir(args);
	if (hredir.nbr_redir == 0)
		return (0);
	while (hredir.nbr_redir != 0)
	{
		redir_path = redir_pathname(args);
		hredir.file_fd = open(redir_path, O_CREAT | O_TRUNC | O_WRONLY, 0777);
		if (hredir.file_fd == -1)
			return (0);
		if (hredir.nbr_redir == 1)
		{
			hredir.orig_fd = dup(STDOUT_FILENO);
			dup2(hredir.file_fd, STDOUT_FILENO);
			close(hredir.file_fd);
			shell->rr = hredir;
			return (1);
		}
		else
		{
			while (args->type != redir)
				args = args->next;
		}
		hredir.nbr_redir--;
	}
	return (0);
}
