/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 16:17:17 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/16 13:03:48 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_minishell *shell)
{
	printf("exit\n");
	if (shell->cmd_split[1] && ft_isdigit(shell->cmd_split[1][0]) == 0)
		ft_fprintf(2, "exit: %s: numeric argument required\n",
			shell->cmd_split[1]);
	else if (shell->cmd_split[1] && shell->cmd_split[2])
	{
		ft_fprintf(2, "exit: too many arguments\n");
		return ;
	}
	free_env(shell->env);
	ft_free_dp((void **)shell->cmd_split);
	exit(EXIT_SUCCESS);
}
