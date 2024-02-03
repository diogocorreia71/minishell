/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_sys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 17:17:17 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 17:45:27 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_fork(int pid)
{
	if (pid < 0)
	{
		perror("fork");
		g_exit_status = 127;
	}
	return (pid);
}

int	check_fd(int fd, char *message)
{
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "Error starting %s fd\n", message);
		perror(message);
		g_exit_status = 127;
	}
	return (fd);
}

int	check_dup(int value)
{
	if (value == -1)
	{
		perror("dup2");
		g_exit_status = 127;
	}
	return (value);
}

int	check_close(int value)
{
	if (value == -1)
	{
		perror("close");
		g_exit_status = 127;
	}
	return (value);
}

int	check_wait(int value)
{
	if (value == -1)
	{
		perror("wait");
		g_exit_status = 127;
	}
	return (value);
}
