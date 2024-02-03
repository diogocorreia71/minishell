/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:17:17 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 02:37:38 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_minishell *shell, t_exec *cmd)
{
	if (shell->in_pipe == NO)
		printf("exit\n");
	if (shell->cmd_split[1] && ft_isdigit(shell->cmd_split[1][0]) == 0)
		ft_fprintf(2, "exit: %s: numeric argument required\n",
			shell->cmd_split[1]);
	else if (shell->cmd_split[1] && shell->cmd_split[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		g_exit_status = 1;
		return ;
	}
	free_env(shell->env);
	free_tree((t_generic *)cmd);
	exit(EXIT_SUCCESS);
}
