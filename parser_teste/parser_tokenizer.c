/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokenizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 02:50:57 by rui               #+#    #+#             */
/*   Updated: 2024/01/21 22:19:16 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

types get_token_type(lst_tokens *token)
{
	if (token == NULL)
		return (DONE);
	else
		return (token->token_type);
}

int	ft_strncmp(char *str1, char *str2, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (0);
	while ((str1[i] == str2[i]) && (i < size - 1) && str1[i] != '\0')
	{
		i++;
	}
	return ((unsigned char)(str1[i]) - (unsigned char)(str2[i]));
}

lst_tokens	*create_node(char *token, types type)
{
	lst_tokens	*node;

	node = (lst_tokens *)malloc(sizeof(lst_tokens));
	if (node == NULL)
		return (NULL);
	node->token = token;
	node->token_type = type;
	node->next = NULL;
	return (node);
}

lst_tokens	*tokenizer()
{
	lst_tokens	*cmds;
	lst_tokens	*head;

	cmds = create_node("ls", word);
	head = cmds;
	cmds->next = create_node(">", redir);
	cmds = cmds->next;
	cmds->next = create_node("dsa", word);
	cmds = cmds->next;
	cmds->next = create_node(">", redir);
	cmds = cmds->next;
	cmds->next = create_node("a", word);
	/* cmds = cmds->next;
	cmds->next = create_node("Hello", word); */
	return (head);
}
