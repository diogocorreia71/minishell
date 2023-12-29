/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:28:35 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/29 12:08:43 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_minishell *shell)
{
	int	i;
	int	flag;

	flag = 0;
	if (!shell->cmd_split[1])
	{
		printf("\n");
		return ;
	}
	if (!ft_strncmp(shell->cmd_split[1], "-n", 3))
		flag = 1;
	i = 1;
	if (flag == 1)
		i = 2;
	while (shell->cmd_split[i])
	{
		printf("%s", shell->cmd_split[i]);
		if (shell->cmd_split[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
}
