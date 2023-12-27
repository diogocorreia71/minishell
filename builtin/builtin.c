/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:04:33 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 10:43:19 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void);
void	cd(t_minishell *shell);
void	echo(t_minishell *shell);
void	export(t_env *env, char **cmd_split);
void	unset(t_env *env, char **cmd_split);
void	env_print(t_env *env, char **cmd_split, int flag);
void	ft_exit(t_minishell *shell);

void	builtin_cmd(t_minishell *shell)
{
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
		ft_exit(shell);
	else
		non_builtin(shell);
}
