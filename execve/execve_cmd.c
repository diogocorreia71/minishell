/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:02:26 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/05 23:13:32 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*exec_path(t_minishell *shell)
{
	char	**bin_dir;
	char	*path1;
	char	*path2;
	int		i;

	i = 0;
	bin_dir = ft_split(get_env(shell->env, "PATH"), ':');
	while (bin_dir && bin_dir[i])
	{
		path1 = ft_strjoin(bin_dir[i], "/");
		path2 = ft_strjoin(path1, shell->cmd_split[0]);
		free(path1);
		if (access(path2, F_OK) == 0)
		{
			ft_free_dp((void **)bin_dir);
			return (path2);
		}
		free(path2);
		i++;
	}
	if (bin_dir)
		ft_free_dp((void **)bin_dir);
	return (ft_strdup(shell->cmd_split[0]));
}

void	ft_execve(t_minishell *shell)
{
	int		status;
	pid_t	pid;
	
	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(STDERR_FILENO, "Error fork (function: non_builtin)\n");
		return ;
	}
	else if (pid == 0)
	{
		if (ft_strncmp(shell->cmd_split[0], "./minishell", 12) == 0)
			change_shlvl(shell->env_array, shell->env);
		execve(shell->path, shell->cmd_split, shell->env_array);
		execve_clean(shell);
		exit(0);
	}
	wait(&status);
	free(shell->path);
	ft_free_dp((void **)shell->env_array);
}
