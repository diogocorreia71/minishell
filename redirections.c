/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:38:34 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/15 12:59:40 by rumachad         ###   ########.fr       */
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

void	free_redir(t_cmd *args)
{
	while (args->type != redin && args->type != redout)
		args = args->next;
	while (args != NULL)
	{
		args->type = ignore;
		args = args->next;
	}
}

void	chtype(t_cmd *args, int *nbr_redin, int *nbr_redout)
{
	while (args != NULL)
	{
		if (args->type == redir)
		{
			if (ft_strncmp(args->token, ">", 2) == 0)
			{
				args->type = redout;
				*nbr_redout = *nbr_redout + 1;
			}
			else
			{
				args->type = redin;
				*nbr_redin = *nbr_redin + 1;
			}
		}
		args = args->next;
	}
}

int	handle_redir(t_minishell *shell, t_cmd *args)
{
	t_rdr	hredir;
	t_cmd	*tmp;
	int		nbr_redir;
	char	*redir_path;
	
	nbr_redir = count_redir(args);
	if (nbr_redir == 0)
		return (0);
	ft_memset((void *)&hredir, 0, sizeof(t_rdr));
	chtype(args, &hredir.nbr_redin, &hredir.nbr_redout);
	if (hredir.nbr_redin && hredir.nbr_redout)
		shell->redir_flag = 3;
	else if (hredir.nbr_redin)
		shell->redir_flag = 2;
	else
		shell->redir_flag = 1;
	tmp = args;
	while (hredir.nbr_redin != 0 || hredir.nbr_redout != 0)
	{
		while (tmp && tmp->type != redin && tmp->type != redout)
			tmp = tmp->next;
		if (tmp == NULL)
			break ;
		redir_path = tmp->next->token;
		if (tmp->type == redin)
		{
			hredir.file_fd = open(redir_path, O_RDONLY);
			if (hredir.file_fd == -1)
				return (0);
			if (hredir.nbr_redin == 1)
			{
				hredir.orig_fd[0] = dup(STDIN_FILENO);
				dup2(hredir.file_fd, STDIN_FILENO);
				close(hredir.file_fd);
			}
			hredir.nbr_redin--;
		}
		else
		{
			hredir.file_fd = open(redir_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (hredir.file_fd == -1)
				return (0);
			if (hredir.nbr_redout == 1)
			{
				hredir.orig_fd[1] = dup(STDOUT_FILENO);
				dup2(hredir.file_fd, STDOUT_FILENO);
				close(hredir.file_fd);
			}
			hredir.nbr_redout--;
		}
		tmp = tmp->next;
	}
	hredir.orig_fd[2] = 0;
	shell->rr = hredir;
	free_redir(args);
	return (0);
}
