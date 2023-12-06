/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:31:42 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/10 15:27:04 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(t_minishell *shell)
{
	t_env	*pwd;
	t_env	*oldpwd;
	
	if (shell->cmd_split[2])
	{
		printf("minishell: cd: too many arguments: %s\n", shell->cmd_split[1]);
		return ;
	}
	if (shell->cmd_split[1][0] == '-' && ft_strlen(shell->cmd_split[1]) == 1)
	{
		free(shell->cmd_split[1]);
		shell->cmd_split[1] = ft_strdup(get_env(shell->env, "OLDPWD"));
		printf("%s\n", shell->cmd_split[1]);
	}
	if (chdir(shell->cmd_split[1]) == -1)
	{
		printf("minishell: cd: no such file or directory: %s\n", shell->cmd_split[1]);
		return ;
	}
	pwd = get_env_node(shell->env, "PWD");
	oldpwd = get_env_node(shell->env, "OLDPWD");
	free(oldpwd->var_value);
	oldpwd->var_value = ft_strdup(pwd->var_value);
	free(pwd->var_value);
	pwd->var_value = getcwd(NULL, 0);
}
