/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:28:35 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 15:21:41 by rumachad         ###   ########.fr       */
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
	int	k;
	int	i;

	i = 1;
	k = 0;
	while (cmd_split[i])
	{
		if (cmd_split[i][0] != '-' || cmd_split[i][1] != 'n')
			break ;
		else if (cmd_split[i][0] == '-' && cmd_split[i][1] == 'n')
		{
			k = 2;
			while (cmd_split[i][k])
			{
				if (cmd_split[i][k] != 'n')
					return (i - 1);
				k++;
			}
		}
		i++;
	}
	return (i - 1);
}

void	echo(t_minishell *shell)
{
	int	i;

	i = 0;
	if (!shell->cmd_split[1])
	{
		printf("\n");
		return ;
	}
	i = echo_syntax(shell->cmd_split);
	while (shell->cmd_split[++i])
	{
		printf("%s", shell->cmd_split[i]);
		if (shell->cmd_split[i + 1] != NULL)
			printf(" ");
	}
	if (!echo_syntax(shell->cmd_split))
		printf("\n");
}
