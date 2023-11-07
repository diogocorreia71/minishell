/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:04:33 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/07 17:08:59 by rumachad         ###   ########.fr       */
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

char	**exec_path(char *cmd_split)
{
	char	**bin;
	char	**path;
	int		i;

	i = 0;
	bin = ft_split(getenv("PATH"), ':');
	while (bin[i])
		i++;
	path = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (bin[i])
	{
		path[i] = ft_strjoin(ft_strjoin(bin[i], "/"),
				cmd_split);
		i++;
	}
	return (path);
}

void	non_builtin(t_minishell *cmds)
{
	char	**bin_path;
	int		status;
	pid_t	pid;
	int		i;
	
	bin_path = exec_path(cmds->cmd_split[0]);
	i = 0;
	while (bin_path[i])
	{
		pid = fork();
		if (pid < 0)
		{
			perror("Error in non_builtin");
			return  ;
		}
		else if (pid == 0)
		{
			if (access(bin_path[i], F_OK) == 0)
				execve(bin_path[i], cmds->cmd_split, cmds->env);
			else
				exit(1);
		}
		wait(&status);
		if (status == 0)
		{
			free(bin_path);		
			return ;
		}
		i++;
	}
	printf("%s: command not found\n", cmds->cmd_str);
	free(bin_path);
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
