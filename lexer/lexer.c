/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:13:02 by rui               #+#    #+#             */
/*   Updated: 2024/01/24 17:31:17 by rumachad         ###   ########.fr       */
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

t_lst_tokens	*create_cmd_token(char *token)
{
	t_lst_tokens	*tokens;
	char			*str;
	int				i;
	int				dquotes;
	int				squotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
	while (token[i])
	{
		if (token[i] == '"' && !squotes)
			dquotes = !dquotes;
		else if (token[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if ((!dquotes && !squotes)
			&& (token[i] == '|'  || token[i] == ' '))
			break ;
		i++;
	}
	str = ft_substr(token, 0, i);
	tokens = create_token(str, WORD);
	free(str);
	return (tokens);
}

char	*lexer_get_var(char *ds_str)
{
	int	i;

	i = 0;
	while (ds_str[i] != ' ' && ds_str[i] != '"'
		&& ds_str[i] != '\'' && ds_str[i])
		i++;
	return (ft_substr(ds_str, 0, i));
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
		else if (cmd[i] == '$')
			tokens->next = create_token(lexer_get_var(cmd + i), EXPAND);
		else
			tokens->next = create_cmd_token(cmd + i);
		tokens = tokens->next;
		while (tokens->token[k + 1])
			k++;
		i = i + k;
	}
	return (head);
}
