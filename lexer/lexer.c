/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:13:02 by rui               #+#    #+#             */
/*   Updated: 2024/02/03 12:46:01 by rumachad         ###   ########.fr       */
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

t_lst_tokens	*create_cmd_token(char *token, t_id	token_type)
{
	t_lst_tokens	*tokens;
	char			*str;
	int				dquotes;
	int				squotes;
	int				i;

	dquotes = 0;
	squotes = 0;
	i = -1;
	while (token[++i])
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
	}
	str = ft_substr(token, 0, i);
	tokens = create_token(str, token_type);
	free(str);
	return (tokens);
}

t_lst_tokens	*create_operator_token(char *operator, int start)
{
	t_lst_tokens	*tokens;

	tokens = NULL;
	if (operator[start] == '|')
		tokens = create_token("|", PIPE);
	else if (operator[start] == '>')
	{
		if (operator[start + 1] && operator[start + 1] == '>')
			tokens = create_token(">>", REDIR);
		else
			tokens = create_token(">", REDIR);
	}
	else if (operator[start] == '<')
	{
		if (operator[start + 1] && operator[start + 1] == '<')
			tokens = create_token("<<", REDIR);
		else
			tokens = create_token("<", REDIR);
	}
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
			continue ;
		else if (cmd[i] == '|' || cmd[i] == '>' || cmd[i] == '<')
			tokens->next = create_operator_token(cmd, i);
		else
			tokens->next = create_cmd_token(cmd + i, WORD);
		tokens = tokens->next;
		while (tokens->token[k + 1])
			k++;
		i = i + k;
	}
	return (head);
}
