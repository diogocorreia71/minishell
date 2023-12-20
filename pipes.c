/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 23:00:23 by rui               #+#    #+#             */
/*   Updated: 2023/12/20 15:59:39 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_minishell *shell)
{
	t_cmd	*args;
	int		nbr_pipes;
	
	args = shell->args;
	nbr_pipes = 0;
	while (args != NULL)
	{
		if (args->type == pipes)
			nbr_pipes++;
		args = args->next;
	}
	return (nbr_pipes);
}

void	init_fd_pipes(t_pipe *info)
{
	int		i;
	
	info->fd = (int **)malloc(sizeof(int *) * (info->nbr_pipes + 1));
	if (info->fd == NULL)
		return (perror("Malloc fd error\n"));
	info->pipe_pid = (pid_t *)malloc(sizeof(pid_t) * (info->nbr_pipes + 1));
	if (info->pipe_pid == NULL)
		return (perror("Malloc pipe_pid error\n"));
	i = -1;
	while (++i < info->nbr_pipes)
	{
		info->fd[i] = (int *)malloc(sizeof(int) * 2);
		if (info->fd[i] == NULL)
			return (perror("Malloc fd 2 error\n"));
	}
	info->fd[i] = 0;
	i = 0;
	while (i < info->nbr_pipes)
	{
		if (pipe(info->fd[i]) == -1)
			return (perror("Pipe creation error\n"));
		i++;
	}
}

void	open_fd(int **fd, int position, int nbr_pipes)
{
	int	i;

	if (position == 0)
		dup2(fd[position][1], STDOUT_FILENO);
	else if (position < nbr_pipes)
	{
		dup2(fd[position - 1][0], STDIN_FILENO);
		dup2(fd[position][1], STDOUT_FILENO);
	}
	else
		dup2(fd[position - 1][0], STDIN_FILENO);
	i = 0;
	while (i < nbr_pipes)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

int	start_pipes(t_minishell *shell, t_pipe *info)
{
	t_cmd	*args;
	int		i;

	i = 0;
	args = shell->args;
	while (i < (info->nbr_pipes + 1))
	{
		info->pipe_pid[i] = fork();
		if (info->pipe_pid[i] == -1)
			perror("Start pipe fork error");
		if (info->pipe_pid[i] == 0)
		{
			lst_to_array(shell, args);
			open_fd(info->fd, i, info->nbr_pipes);
			builtin_cmd(shell);
			ft_free_dp((void **)(shell->cmd_split));
			exit(0);
		}
		while (args != NULL && args->type != pipes)
			args = args->next;
		if (args != NULL && args->type == pipes)
			args = args->next;
		i++;
	}
	return (0);
}
