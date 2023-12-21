/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:17:17 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/21 16:17:38 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_minishell *shell)
{
	printf("exit\n");
	if (shell->cmd_split[1] && ft_isdigit(shell->cmd_split[1][0]) == 0)
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(shell->cmd_split[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
	else if (shell->cmd_split[1] && shell->cmd_split[2])
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return ;
	}
	clean_program(shell);
	free_env(shell->env);
	exit(EXIT_SUCCESS);
}
