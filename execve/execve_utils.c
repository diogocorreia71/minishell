/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:35:31 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/03 14:52:23 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_error(int error)
{
	if (error == 1)
		ft_putstr_fd(": filename argument required\n", STDERR_FILENO);
	else if (error == 2)
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	else if (error == 3)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (error == 4)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
}
int	is_dir_err(char *cmd)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (cmd[i])
	{
		if ((cmd[i] != '/' && cmd[i] != '.') || counter > 2)
			break;
		else if (cmd[i] == '.')
			counter++;
		else if (cmd[i] == '/')
			counter = 0;
		i++;
	}
	if (counter <= 2 && cmd[i] == '\0')
		return (true);
	return (false);
}

int	execve_syntax(char *cmd, t_env *env, char *path)
{
	if (ft_strlen(cmd) == 1 && cmd[0] == '.')
		return (1);
	else if (ft_strchr(cmd, '/') && is_dir_err(cmd) == 1)
		return (2);
	else if ((access(path, F_OK) == -1 && (ft_strchr(cmd, '/') ||
		!get_env(env, "PATH"))))
		return (3);
	else if (!access(path, F_OK) && access(path, X_OK) == -1)
		return (4);
	else if (access(path, F_OK | X_OK) == -1
		|| (cmd[0] == '.' && cmd[1] == '.'))
		return (5);
	return (0);
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
