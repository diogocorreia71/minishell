/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 12:56:04 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 12:56:25 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_id	unclosed_quotes(char *rl_str)
{
	if (handle_quotes(rl_str) == 1)
	{
		ft_fprintf(STDERR_FILENO, "Invalid Quotes\n");
		free(rl_str);
		return (YES);
	}
	return (NO);
}

t_id	token_has_quotes(char *token)
{
	int	i;

	i = 0;
	if (token == NULL)
		return (NO);
	while (token[i])
	{
		if (token[i] == '"' || token[i] == '\'')
			return (YES);
		i++;
	}
	return (NO);
}
