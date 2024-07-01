/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:31:42 by rumachad          #+#    #+#             */
/*   Updated: 2024/07/01 01:43:06 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cd_syntax(t_env *env, char **cmd_split, char **key)
{
	char	*path;

	*key = NULL;
	if (cmd_split[1] == NULL)
		*key = "HOME";
	else if (ft_strncmp(cmd_split[1], "-", 2) == 0)
		*key = "OLDPWD";
	else if (ft_strncmp(cmd_split[1], "--", 3) == 0)
		*key = "HOME";
	if (*key != NULL)
		path = get_env_val(env, *key);
	else
	{
		path = ft_strdup(cmd_split[1]);
		*key = "\0";
	}
	return (path);
}

void	cd_env_update(t_env *env)
{
	t_env	*pwd;
	t_env	*oldpwd;

	pwd = get_env_node(env, "PWD");
	if (pwd == NULL)
		return ;
	oldpwd = get_env_node(env, "OLDPWD");
	if (oldpwd == NULL)
		return ;
	free(oldpwd->var_value);
	oldpwd->var_value = ft_strdup(pwd->var_value);
	free(pwd->var_value);
	pwd->var_value = getcwd(NULL, 0);
}

void	cd_error_print(int error, char *key, char *path)
{
	if (error == 1)
		ft_fprintf(STDERR_FILENO, "cd: too many arguments\n");
	else if (error == 2)
		ft_fprintf(STDERR_FILENO, "cd: %s not set", key);
	else if (error == 3)
	{
		ft_fprintf(STDERR_FILENO, "cd: %s: No such file or directory\n", path);
		free(path);
	}
	g_exit_status = 1;
}

void	cd(t_minishell *shell)
{
	char	*path;
	char	*key;

	if (shell->cmd_split[1] && shell->cmd_split[2] != 0)
	{
		cd_error_print(1, NULL, NULL);
		return ;
	}
	path = cd_syntax(shell->env, shell->cmd_split, &key);
	if (path == NULL)
	{
		cd_error_print(2, key, NULL);
		return ;
	}
	if (chdir(path) == -1)
	{
		cd_error_print(3, NULL, path);
		return ;
	}
	if (ft_strncmp(key, "OLDPWD", 7) == 0)
		printf("%s\n", path);
	free(path);
	cd_env_update(shell->env);
}
