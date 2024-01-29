/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:02:26 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/29 13:14:47 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*exec_path(t_minishell *shell)
{
	char		**bin_dir;
	char		*path1;
	char		*path2;
	int			i;
	struct stat buffer;
	

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

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		g_exit_status = 128 + SIGINT;
	}
	else if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		g_exit_status = 128 + SIGQUIT;
	}
}

void	ft_execve(t_minishell *shell)
{
	pid_t	pid;
	int		status;
	
	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(STDERR_FILENO, "Error fork (function: execve)\n");
		return ;
	}
	else if (pid == 0)
	{
		if (ft_strncmp(shell->cmd_split[0], "./minishell", 12) == 0)
			change_shlvl(shell->env_array, shell->env);
		execve(shell->path, shell->cmd_split, shell->env_array);
	}
	waitpid(pid, &status, 0);
	g_exit_status = WEXITSTATUS(status);
	free(shell->path);
	ft_free_dp((void **)shell->env_array);
}
