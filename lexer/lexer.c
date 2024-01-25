/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:13:02 by rui               #+#    #+#             */
/*   Updated: 2024/01/25 01:44:16 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst_tokens	*create_token(char *token, t_id type)
{
	t_lst_tokens	*node;

	node = malloc(sizeof(t_lst_tokens) * 1);
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->token = ft_strdup(token);
	node->next = NULL;
	return (node);
}

t_id	finish_token(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == ' ')
		return (YES);
	return (NO);
}

t_lst_tokens	*create_cmd_token(char *token)
{
	t_lst_tokens	*tokens;
	t_id			token_type;
	char			*str;
	int				i;
	int				dquotes;
	int				squotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
	token_type = WORD;
	while (token[i])
	{
		if (!squotes && (token[i] == '$'
			|| (!dquotes && token[i] == '~')))
			token_type = EXPAND;
		if (token[i] == '"' && !squotes)
			dquotes = !dquotes;
		else if (token[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if ((!dquotes && !squotes) && finish_token(token[i]) == YES)
			break ;
		i++;
	}
	str = ft_substr(token, 0, i);
	tokens = create_token(str, token_type);
	free(str);
	return (tokens);
}

t_lst_tokens	*make_tokens(t_minishell *shell, t_lst_tokens *tokens)
{
	t_lst_tokens	*head;
	int				i;
	int				k;
	char			*cmd;

	i = -1;
	cmd = shell->rl_str;
	tokens = malloc(sizeof(t_lst_tokens) * 1);
	head = tokens;
	while (cmd[++i])
	{
		k = 0;
		if (is_space(cmd[i]) == YES)
			continue;
		else if (cmd[i] == '|')
			tokens->next = create_token("|", PIPE);
		else if (cmd[i] == '>')
		{
			if (cmd[i + 1] && cmd[i + 1] == '>')
				tokens->next = create_token(">>", REDIR);
			else
				tokens->next = create_token(">", REDIR);
		}
		else if (cmd[i] == '<')
		{
			if (cmd[i + 1] && cmd[i + 1] == '<')
				tokens->next = create_token("<<", REDIR);
			else
				tokens->next = create_token("<", REDIR);
		}
		else
			tokens->next = create_cmd_token(cmd + i);
		tokens = tokens->next;
		while (tokens->token[k + 1])
			k++;
		i = i + k;
	}
	return (head);
}
