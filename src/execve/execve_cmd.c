/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:02:26 by rumachad          #+#    #+#             */
/*   Updated: 2024/07/01 00:15:36 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*exec_path(t_minishell *shell)
{
	char		**bin_dir;
	char		*path1;
	char		*path2;
	int			i;
	struct stat	buffer;

	i = -1;
	bin_dir = ft_split(get_env(shell->env, "PATH"), ':');
	while (bin_dir && bin_dir[++i])
	{
		path1 = ft_strjoin(bin_dir[i], "/");
		path2 = ft_strjoin(path1, shell->cmd_split[0]);
		free(path1);
		if (!stat(path2, &buffer) && S_ISREG(buffer.st_mode) == 1)
		{
			ft_free_dp((void **)bin_dir);
			return (path2);
		}
		free(path2);
	}
	if (bin_dir)
		ft_free_dp((void **)bin_dir);
	return (ft_strdup(shell->cmd_split[0]));
}

void	exec_exit_status(int status)
{
	int	exit_status;

	exit_status = 0;
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		g_exit_status = exit_status;
	}
	else if (WIFSIGNALED(status))
	{
		exit_status = WTERMSIG(status);
		if (exit_status == 3)
			printf("Quit (core dumped)");
		printf("\n");
		g_exit_status = 128 + exit_status;
	}
}

void	ft_execve(t_minishell *shell)
{
	pid_t	pid;
	int		status;

	status = 0;
	shell->path = exec_path(shell);
	shell->env_array = array_env(shell->env);
	pid = fork();
	if (check_fork(pid) < 0)
		return ;
	else if (pid == 0)
	{
		init_signals(SIGCHILD);
		execve(shell->path, shell->cmd_split, shell->env_array);
		execve_error(shell, shell->path);
	}
	init_signals(SIGIGNORE);
	check_wait(waitpid(pid, &status, 0));
	exec_exit_status(status);
	free(shell->path);
	ft_free_dp((void **)shell->env_array);
}
