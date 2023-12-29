/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeve_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 11:02:26 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/29 12:04:54 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exec(t_minishell *shell, char *path)
{
	ft_putstr_fd(shell->cmd_split[0], STDERR_FILENO);
	if (!path)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (!access(path, F_OK) && access(path, X_OK) == -1)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (!access(path, F_OK && X_OK) && shell->rl_str[0] == '.')
		ft_putstr_fd(": filename argument required\n", STDERR_FILENO);
	else if (!access(path, F_OK && X_OK) && shell->rl_str[0] == '/')
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	else if (access(path, F_OK) == -1)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(1);
}

char	*exec_path(t_minishell *shell)
{
	char	**bin_dir;
	char	*path1;
	char	*path2;
	int		i;

	i = 0;
	bin_dir = ft_split(get_env(shell->env, "PATH"), ':');
	if (bin_dir == NULL)
		return (NULL);
	while (bin_dir[i])
	{
		path1 = ft_strjoin(bin_dir[i], "/");
		path2 = ft_strjoin(path1, shell->cmd_split[0]);
		free(path1);
		if (access(path2, F_OK) == 0)
		{
			ft_free_dp((void **)bin_dir);
			return (path2);
		}
		free(path2);
		i++;
	}
	if (bin_dir)
		ft_free_dp((void **)bin_dir);
	return (ft_strdup(shell->cmd_split[0]));
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

void	non_builtin(t_minishell *shell)
{
	char	*path;
	int		status;
	pid_t	pid;
	
	path = NULL;
	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(2, "Error fork (function: non_builtin)\n");
		return ;
	}
	else if (pid == 0)
	{
		path = exec_path(shell);
		if (ft_strncmp(shell->cmd_split[0], "./minishell", 12) == 0)
			change_shlvl(shell->env_array, shell->env);
		execve(path, shell->cmd_split, shell->env_array);
		check_exec(shell, path);
		ft_free_dp((void **)shell->env_array);
	}
	wait(&status);
	free(path);
}
