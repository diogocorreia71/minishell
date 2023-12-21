/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:31:42 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/21 16:15:09 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_syntax(t_minishell *shell, char **path)
{
	if (shell->cmd_split[1] == NULL)
	{
		*path = get_env_val(shell->env, "HOME");
		if (*path == NULL)
			ft_putstr_fd("cd: HOME is not set\n", STDERR_FILENO);
	}
	else if (shell->cmd_split[1][0] == '-' && ft_strlen(shell->cmd_split[1]) == 1)
	{
		*path = get_env_val(shell->env, "OLDPWD");
		if (*path == NULL)
			ft_putstr_fd("cd: OLDPWD is not set\n", STDERR_FILENO);
		printf("%s\n", *path);
	}
	else if (shell->cmd_split[2])
		printf("cd: too many arguments: %s\n", shell->cmd_split[1]);
	else
	{
		*path = ft_strdup(shell->cmd_split[1]);
		return(0);
	}
	return (1);
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
	
	if (cd_syntax(shell, &path) == 1)
		return ;
	if (chdir(path) == -1)
	{
		printf("cd: %s: no such file or directory\n", path);
		free(path);
		return ;
	}
	free(path);
	//Para atualizar o env
	cd_env_update(shell->env);
}
