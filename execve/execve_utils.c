/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:35:31 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/20 14:50:57 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_error(t_env *env, char *path)
{
	int	file_check;
	int	exe_check;
	int	write_check;

	file_check = access(path, F_OK);
	write_check = access(path, W_OK);
	exe_check = access(path, X_OK);
	ft_fprintf(STDERR_FILENO, "%s", path);
	if (!file_check && exe_check && ft_strchr(path, '/'))
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (!file_check && !exe_check && path[0] != '.')
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	else if (ft_strchr(path, '/') || !get_env(env, "PATH"))
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	g_exit_status = 127 - ((!file_check && exe_check) || !write_check);
	exit(g_exit_status);
}

void	change_shlvl(char **env_array, t_env *env)
{
	char	*shlvl_char;
	int		shlvl;
	int		i;
	int		k;

	i = 0;
	while (ft_strncmp(env->var, "SHLVL", 6) != 0)
	{
		env = env->next;
		i++;
	}
	k = 0;
	while (env_array[i][k + 1])
		k++;
	shlvl = ft_atoi(&env_array[i][k]);
	shlvl++;
	shlvl_char = ft_itoa(shlvl);
	free(env_array[i]);
	env_array[i] = ft_strjoin("SHLVL=", shlvl_char);
}
