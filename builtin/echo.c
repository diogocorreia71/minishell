/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:28:35 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/15 14:26:43 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n(char c)
{
	if (c == 'n')
		return (1);
	else
		return (0);
}

int	echo_syntax(char **cmd_split)
{
	int	i;
	int	check;

	if (!cmd_split[1])
		return (printf("\n"), -1);
	i = 1;
	check = 0;
	if (cmd_split[1][0] == '-')
	{
		while (cmd_split[1][i])
		{
			check = is_n(cmd_split[1][i]);
			if (check == 0)
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

void	echo(t_minishell *shell)
{
	int	i;
	int	flag;

	i = 1;
	flag = echo_syntax(shell->cmd_split);
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
