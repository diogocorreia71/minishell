/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:02:26 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/09 12:21:42 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exec(t_minishell *cmds, char *path)
{
	if (!path)
		printf("%s: No such file or directory\n", cmds->cmd_str);
	else if (!access(path, F_OK) && access(path, X_OK) == -1)
		printf("%s: Permission denied\n", cmds->cmd_str);
	else if (!access(path, F_OK && X_OK) && cmds->cmd_str[0] == '.')
		printf("%s: filename argument required\n", cmds->cmd_str);
	else if (!access(path, F_OK && X_OK) && cmds->cmd_str[0] == '/')
		printf("%s: Is a directory\n", cmds->cmd_str);
	else if (access(path, F_OK) == -1)
		printf("%s: command not found\n", cmds->cmd_str);
	exit(1);
}

char	*exec_path(t_minishell *cmds)
{
	char	**bin_dir;
	char	*path1;
	char	*path2;
	int		i;

	i = 0;
	bin_dir = ft_split(get_env(cmds->env, "PATH"), ':');
	if (bin_dir == NULL)
		return (NULL);
	while (bin_dir[i])
	{
		path1 = ft_strjoin(bin_dir[i], "/");
		path2 = ft_strjoin(path1, cmds->cmd_split[0]);
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
	return (ft_strdup(cmds->cmd_split[0]));
}

void	non_builtin(t_minishell *cmds)
{
	char	*path;
	int		status;
	pid_t	pid;
	
	path = NULL;
	pid = fork();
	if (pid < 0)
	{
		perror("Error creating fork (function: non_builtin)");
		return  ;
	}
	else if (pid == 0)
	{
		path = exec_path(cmds);
		execve(path, cmds->cmd_split, cmds->env_array);
		check_exec(cmds, path);
	}
	wait(&status);
	free(path);
}
