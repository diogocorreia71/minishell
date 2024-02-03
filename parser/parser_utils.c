/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:01 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 01:44:21 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_id	get_token_type(t_lst_tokens *arg)
{
	if (arg == NULL)
		return (DONE);
	else
		return (arg->type);
}

t_id	get_redir_type(char *token, t_id token_type)
{
	t_id	type;

	type = token_type;
	if (ft_strncmp(token, ">", 2) == 0)
		type = REDIR_OUT;
	else if (ft_strncmp(token, "<", 2) == 0)
		type = REDIR_IN;
	else if (ft_strncmp(token, ">>", 3) == 0)
		type = APPEND;
	else if (ft_strncmp(token, "<<", 3) == 0)
		type = HERE_DOC;
	return (type);
}

int	count_tokens(t_lst_tokens *args)
{
	int	count;

	count = 0;
	while (args && args->type != PIPE)
	{
		if (args->type == REDIR)
		{
			args = args->next;
			if (args)
				args = args->next;
			continue ;
		}
		count++;
		args = args->next;
	}
	return (count);
}
