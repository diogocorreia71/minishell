/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:31:44 by rui               #+#    #+#             */
/*   Updated: 2024/02/03 17:55:23 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	if (c == ' ')
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
