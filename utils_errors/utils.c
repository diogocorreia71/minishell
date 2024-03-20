/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:31:44 by rui               #+#    #+#             */
/*   Updated: 2024/03/20 16:06:49 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *cmd)
{
	if (cmd[0] == '-')
	{
		ft_fprintf(STDERR_FILENO, "Invalid Option\n");
		return (1);
	}
	return (0);
}

int	space_input(char *str)
{
	int	i;

	i = 0;
	while ((str[i] == 32 || str[i] == 9) && str[i])
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == 9)
		return (YES);
	else
		return (NO);
}

void	free_first(t_lst_tokens **tokens)
{
	t_lst_tokens	*tmp;

	tmp = *tokens;
	*tokens = (*tokens)->next;
	free(tmp);
}

void	close_fd(int pipe_fd[2])
{
	check_close(close(pipe_fd[0]));
	check_close(close(pipe_fd[1]));
}
