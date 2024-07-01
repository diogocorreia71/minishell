/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:17:17 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/20 11:51:59 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	number_signal(char c)
{
	if (c == '+' || c == '-')
		return (YES);
	return (NO);
}

int	check_exit(char **cmd_split, int *status)
{
	int	i;

	i = 0;
	if (!cmd_split[1])
		return (0);
	*status = ft_atoi(cmd_split[1]);
	if (number_signal(cmd_split[1][0]))
		i = 1;
	while (cmd_split[1][i])
	{
		if (ft_isdigit(cmd_split[1][i]) == 0)
			return (-1);
		i++;
	}
	return (0);
}

int	check_exit_args(char **cmd_split, int status)
{
	if (status < 0)
		g_exit_status = 256 + status;
	else
		g_exit_status = status;
	if (cmd_split[1] && cmd_split[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		g_exit_status = 1;
		return (-1);
	}
	return (0);
}

void	ft_exit(t_minishell *shell, t_exec *cmd)
{
	int	status;

	status = 0;
	if (check_exit(shell->cmd_split, &status) == -1)
	{
		ft_fprintf(STDERR_FILENO, "%s: numeric argument required\n",
			shell->cmd_split[1]);
		g_exit_status = 2;
	}
	else if (check_exit_args(shell->cmd_split, status) == -1)
		return ;
	if (shell->in_pipe == NO)
		printf("exit\n");
	free_env(shell->env);
	free_tree((t_gen *)cmd);
	exit(g_exit_status);
}
