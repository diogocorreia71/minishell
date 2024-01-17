/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 23:00:23 by rui               #+#    #+#             */
/*   Updated: 2024/01/17 15:55:39 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_cmd *args)
{
	t_cmd	*tmp;
	int		nbr_pipes;
	
	tmp = args;
	nbr_pipes = 0;
	while (tmp != NULL)
	{
		if (tmp->type == pipes)
			nbr_pipes++;
		tmp = tmp->next;
	}
	return (nbr_pipes);
}

int	init_fd_pipes(t_pipe *info)
{
	int		i;
	
	info->fd = (int **)malloc(sizeof(int *) * (info->nbr_pipes + 1));
	if (info->fd == NULL)
		return (ft_fprintf(STDERR_FILENO, "Malloc fd error\n"));
	info->pipe_pid = (pid_t *)malloc(sizeof(pid_t) * (info->nbr_pipes + 1));
	if (info->pipe_pid == NULL)
		return (ft_fprintf(STDERR_FILENO, "Malloc pipe_pid error\n"));
	i = -1;
	while (++i < info->nbr_pipes)
	{
		info->fd[i] = (int *)malloc(sizeof(int) * 2);
		if (info->fd[i] == NULL)
			return (ft_fprintf(STDERR_FILENO, "Malloc fd 2 error\n"));
	}
	info->fd[i] = 0;
	i = 0;
	while (i < info->nbr_pipes)
	{
		if (pipe(info->fd[i]) == -1)
			return (ft_fprintf(STDERR_FILENO, "Pipe creation error\n"));
		i++;
	}
	return (0);
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

int	start_pipes(t_minishell *shell, t_pipe *info, t_cmd *args)
{
	int		i;

	i = -1;
	while (++i < (info->nbr_pipes + 1))
	{
		info->pipe_pid[i] = fork();
		if (info->pipe_pid[i] == -1)
			ft_fprintf(STDERR_FILENO, "Start pipe fork error\n");
		if (info->pipe_pid[i] == 0)
		{
			open_fd(info->fd, i, info->nbr_pipes);
			if (shell->nbr_redir != 0)
				if (start_redir(args) == -1)
					exit(1);
			if (lst_to_array(shell, args) != 0)
				exit(1);
			builtin_cmd(shell);
			free_all_child(shell, info);
			exit(0);
		}
		while (args != NULL && args->type != pipes)
			free_prev_node(&args);
		if (args != NULL && args->type == pipes)
			free_prev_node(&args);
	}
	return (0);
}
