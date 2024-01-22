/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:04:33 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/22 17:53:24 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void);
void	cd(t_minishell *shell);
void	echo(t_minishell *shell);
void	export(t_env *env, char **cmd_split);
void	unset(t_env *env, char **cmd_split);
void	env_print(t_env *env, char **cmd_split, int flag);
void	ft_exit(t_minishell *shell, t_exec *cmd);

int	non_builtin(t_minishell *shell)
{
	int		error;

	shell->env_array = array_env(shell->env);
	shell->path = exec_path(shell);
	error = execve_syntax(shell->cmd_split[0], shell->env, shell->path);
	if (error != 0)
	{
		ft_putstr_fd(shell->cmd_split[0], STDERR_FILENO);
		execve_error(error);
		free(shell->path);
		ft_free_dp((void **)shell->cmd_split);
		ft_free_dp((void **)shell->env_array);
		return (-1);
	}
	return (0);
}

t_id	is_builtin(char *command)
{
	if (!ft_strncmp(command, "pwd", 4))
		return (YES);
	else if (!ft_strncmp(command, "echo", 5))
		return (YES);
	else if (!ft_strncmp(command, "cd", 3))
		return (YES);
	else if (!ft_strncmp(command, "env", 4))
		return (YES);
	else if (!ft_strncmp(command, "unset", 6))
		return (YES);
	else if (!ft_strncmp(command, "export", 7))
		return (YES);
	else if (!ft_strncmp(command, "exit", 5))
		return (YES);
	return (NO);
}

void	builtin_cmd(t_minishell *shell, t_exec *cmd)
{
	if (shell->cmd_split[0] == 0)
		return ;
	if (!ft_strncmp(shell->cmd_split[0], "pwd", 4))
		pwd();
	else if (!ft_strncmp(shell->cmd_split[0], "echo", 5))
		echo(shell);
	else if (!ft_strncmp(shell->cmd_split[0], "cd", 3))
		cd(shell);
	else if (!ft_strncmp(shell->cmd_split[0], "env", 4))
		env_print(shell->env, shell->cmd_split, 0);
	else if (!ft_strncmp(shell->cmd_split[0], "unset", 6))
		unset(shell->env, shell->cmd_split);
	else if (!ft_strncmp(shell->cmd_split[0], "export", 7))
		export(shell->env, shell->cmd_split);
	else if (!ft_strncmp(shell->cmd_split[0], "exit", 5))
		ft_exit(shell, cmd);
}
