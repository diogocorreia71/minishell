/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:04:33 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/08 00:45:59 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	char	*dir;
	
	dir = getcwd(NULL, 0);
	printf("%s\n", dir);
	free(dir);
}

void	echo(char **cmd_split)
{
	int	i;

	i = 1;
	while (cmd_split[i])
	{
		printf("%s", cmd_split[i]);
		if (cmd_split[i + 1] != NULL)
			printf(" ");
		i++;
	}
	printf("\n");
}

void	cd(char **cmd_split)
{
	if (cmd_split[2])
	{
		printf("cd: string not in pwd: %s\n", cmd_split[1]);
		return ;
	}
	else if (chdir(cmd_split[1]) == -1)
		printf("cd: no such file or directory: %s\n", cmd_split[1]);
}

void	env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

char	*exec_path(char *cmd)
{
	char	**bin_dir;
	char	*path1;
	char	*path2;
	int		i;

	if (ft_strchr("/.", cmd[0]))
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
		else
			return (NULL);
	}
	i = 0;
	bin_dir = ft_split(getenv("PATH"), ':');
	while (bin_dir[i])
	{
		path1 = ft_strjoin(bin_dir[i], "/");
		path2 = ft_strjoin(path1, cmd);
		free(path1);
		if (access(path2, F_OK) == 0)
			return (path2);
		i++;
	}
	return (NULL);
}

void	non_builtin(t_minishell *cmds)
{
	char	*path;
	int		status;
	pid_t	pid;
	
	path = exec_path(cmds->cmd_split[0]);
	if (path == NULL)
	{
		printf("%s: command not found\n", cmds->cmd_str);
		free(path);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Error creating fork (function: non_builtin)");
		return  ;
	}
	else if (pid == 0)
		execve(path, cmds->cmd_split, cmds->env);
	wait(&status);
}

void	builtin_cmd(t_minishell *cmds)
{
	if (!ft_strncmp(cmds->cmd_split[0], "pwd", 4))
		pwd();
	else if (!ft_strncmp(cmds->cmd_split[0], "echo", 5))
		echo(cmds->cmd_split);
	else if (!ft_strncmp(cmds->cmd_split[0], "cd", 3))
		cd(cmds->cmd_split);
	else if (!ft_strncmp(cmds->cmd_split[0], "env", 4))
		env(cmds->env);
	else if (!ft_strncmp(cmds->cmd_split[0], "exit", 5))
	{
		clean_program(cmds);
		exit(EXIT_SUCCESS);
	}
	else
		non_builtin(cmds);
}
