/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 23:00:23 by rui               #+#    #+#             */
/*   Updated: 2023/12/19 23:10:47 by rui              ###   ########.fr       */
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

void	init_fd(int *fd, int flag)
{
	if (flag == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
	}
}

pid_t	*init_pipes(int *fd, int nbr_pipes)
{
	pid_t	*pipe_pid;
	int		i;
	
	i = 0;
	while (i < nbr_pipes)
	{
		if (pipe(fd) == -1)
			return (NULL);
		i++;
	}
	pipe_pid = (pid_t *)malloc(sizeof(pid_t) * (nbr_pipes + 2));
	return (pipe_pid);
}

int	start_pipes(t_minishell *shell, int nbr_pipes)
{
	pid_t	*pipe_pid;
	t_cmd	*args;
	int		fd[2];
	int		i;

	pipe_pid = init_pipes(fd, nbr_pipes);
	if (pipe_pid == NULL)
		return (1);
	i = 0;
	args = shell->args;
	while (i < nbr_pipes + 1)
	{
		pipe_pid[i] = fork();
		if (pipe_pid[i] == -1)
			return (2);
		if (pipe_pid[i] == 0)
		{
			lst_to_array(shell, args);
			if (i == nbr_pipes)
				init_fd(fd, 0);
			else
				init_fd(fd, 1);
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
	close(fd[0]);
	close(fd[1]);
	i = 0;
	while (i < nbr_pipes + 1)
		waitpid(pipe_pid[i++], NULL, 0);
	free(pipe_pid);
	return (0);
}
