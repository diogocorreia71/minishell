/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:38:34 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/17 15:58:58 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_cmd *args)
{
	while (args->type != redin && args->type != redout && args->type != append)
		args = args->next;
	while (args != NULL)
	{
		args->type = ignore;
		args = args->next;
	}
}

int	set_flags(t_type type)
{
	if (type == redout)
		return (O_CREAT | O_TRUNC | O_WRONLY);
	else if (type == append)
		return (O_CREAT | O_APPEND | O_WRONLY);
	else
		return (O_RDONLY);
}

void	prep_fd(int file_fd, t_type type)
{
	int	fd;

	fd = 0;
	if (type == redout || type == append)
		fd = 1;
	else if (type == redin)
		fd = 0;
	dup2(file_fd, fd);
	close(file_fd);
}

int	set_up_file(char *redir_path, t_type type, int exe_redir)
{
	int	flags;
	int	file_fd;
	
	flags = set_flags(type);
	if (flags == 0)
		if (access(redir_path, F_OK) == -1)
		{
			ft_fprintf(STDERR_FILENO, "%s: No such file or directory\n", redir_path);
			return (-1);
		}
	file_fd = open(redir_path, flags, 0644);
	if (exe_redir == true)
		prep_fd(file_fd, type);
	return (file_fd);
}

int	see(t_cmd *args, t_type type)
{
	if (args->next)
		args = args->next;
	else
		return (true);
	while (args)
	{
		if (args->type == type)
			return (false);
		args = args->next;
	}
	return (true);
}


int	start_redir(t_cmd *args)
{
	t_cmd	*head;
	char	*redir_path;
	int		file_fd;

	head = args;
	while (args != NULL)
	{
		if (args->type == redout || args->type == redin || args->type == append)
		{
			redir_path = args->next->token;
			file_fd = set_up_file(redir_path, args->type,
				see(args, args->type));
			if (file_fd == -1)
				return (-1);
		}
		args = args->next;
	}
	free_redir(head);
	return (0);
}
