/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:31:42 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 11:32:42 by rumachad         ###   ########.fr       */
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
	oldpwd = get_env_node(env, "OLDPWD");
	free(oldpwd->var_value);
	oldpwd->var_value = ft_strdup(pwd->var_value);
	free(pwd->var_value);
	pwd->var_value = getcwd(NULL, 0);
}

void	cd(t_minishell *shell)
{
	char	*path;
	char	*key;

	if (shell->cmd_split[2])
	{
		ft_fprintf(2, "cd: too many arguments\n");
		return ;
	}
	path = cd_syntax(shell->env, shell->cmd_split, &key);
	if (path == NULL)
	{
		ft_fprintf(2, "cd: %s not set", key);
		return ;
	}
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "cd: %s: no such file or directory\n", path);
		free(path);
		return ;
	}
	if (ft_strncmp(key, "OLDPWD", 7) == 0)
		printf("%s\n", path);
	free(path);
	cd_env_update(shell->env);
}
