/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:35:31 by rumachad          #+#    #+#             */
/*   Updated: 2024/07/01 01:42:12 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_error(t_minishell *shell, char *path)
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
	else if (ft_strchr(path, '/') || !get_env(shell->env, "PATH"))
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	g_exit_status = 127 - ((!file_check && exe_check) || !write_check);
	free(path);
	ft_free_dp((void **)shell->env_array);
	free_child(shell->ast_head, shell->env);
	exit(g_exit_status);
}

void	change_shlvl(t_env **env)
{
	t_env	*shlvl_node;
	int		curr_shlvl_int;
	char	*next_shlvl;

	shlvl_node = get_env_node(*env, "SHLVL");
	if (shlvl_node == NULL)
	{
		*env = create_node_env("SHLVL", "=1", 1);
		return ;
	}
	curr_shlvl_int = ft_atoi(shlvl_node->var_value);
	curr_shlvl_int++;
	next_shlvl = ft_itoa(curr_shlvl_int);
	if (next_shlvl == NULL)
		return ;
	free(shlvl_node->var_value);
	shlvl_node->var_value = next_shlvl;
}
