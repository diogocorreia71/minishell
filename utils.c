/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:31:44 by rui               #+#    #+#             */
/*   Updated: 2023/11/26 19:32:26 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	if (c == ' ')
		return (1);
	else
		return (0);
}

void	free_first(t_cmd **tokens)
{
	t_cmd	*tmp;

	tmp = *tokens;
	*tokens = (*tokens)->next;
	free(tmp);
}
