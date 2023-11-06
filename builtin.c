/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:04:33 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/06 16:47:06 by rumachad         ###   ########.fr       */
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

void	non_builtin(t_minishell *cmds)
{
	char	*bin_path;

	bin_path = ft_strjoin("/usr/bin/", cmds->cmd_split[0]);
	execve(bin_path, cmds->cmd_split, cmds->env);
	printf("%s: command not found\n", cmds->cmd_str);
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
