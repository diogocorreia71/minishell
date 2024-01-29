/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:31:44 by rui               #+#    #+#             */
/*   Updated: 2024/01/26 15:50:06 by rui              ###   ########.fr       */
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
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
