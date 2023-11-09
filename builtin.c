/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:04:33 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/09 17:31:12 by rumachad         ###   ########.fr       */
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

void	env(t_env *env)
{
	while (env)
	{
		printf("%s=", env->var);
		printf("%s\n", env->var_value);
		env = env->next;
	}
}

void	unset(t_env *env, char *cmd)
{
	t_env	*tmp;

	if (!get_env(env, cmd))
		return ;
	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strcmp(cmd, tmp->var) == 0)
			break ;
		tmp = tmp->next;
	}
	while (env->next->var != tmp->var)
		env = env->next;
	env->next =env->next->next;
	free(tmp->var);
	free(tmp->var_value);
	free(tmp);
}

void	export(t_env *env, char *cmd)
{
	char	*var;
	int		k;
	int		i;

	i = 0;
	while (cmd[i] != '=')
		i++;
	var = (char *)malloc(sizeof(char) * (i + 1));
	k = -1;
	while (++k < i)
		var[k] = cmd[k];
	if (get_env(env, var))
		unset(env, var);
	env = env_last(env);
	env->next = create_node(var, cmd);
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
	else if (!ft_strncmp(cmds->cmd_split[0], "unset", 6))
		unset(cmds->env, cmds->cmd_split[1]);
	else if (!ft_strncmp(cmds->cmd_split[0], "export", 7))
		export(cmds->env, cmds->cmd_split[1]);
	else if (!ft_strncmp(cmds->cmd_split[0], "exit", 5))
	{
		clean_program(cmds);
		free_env(cmds->env);
		exit(EXIT_SUCCESS);
	}
	else
		non_builtin(cmds);
}
