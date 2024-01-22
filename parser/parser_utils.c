/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:01 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/22 11:40:17 by rumachad         ###   ########.fr       */
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

t_id	get_redir_type(char *token)
{
	t_id	type;

	type = WORD;
	if (ft_strncmp(token, ">", 2) == 0)
		type = REDIR_OUT;
	else if (ft_strncmp(token, "<", 2) == 0)
		type = REDIR_IN;
	else if (ft_strncmp(token, ">>", 3) == 0)
		type = APPEND;
	return (type);
}
