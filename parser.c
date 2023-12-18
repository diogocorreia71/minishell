/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:13:02 by rui               #+#    #+#             */
/*   Updated: 2023/12/18 11:25:45 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_token(char *token, t_type type)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->token = ft_strdup(token);
	node->next = NULL;
	return (node);
}

t_cmd	*create_cmd_token(char *token)
{
	t_cmd	*tokens;
	char	*str;
	int		i;
	int		dquotes;
	int		squotes;

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
	tokens = create_token(str, words);
	free(str);
	return (tokens);
}

t_cmd	*make_tokens(t_minishell *shell, t_cmd *tokens)
{
	t_cmd	*tmp;
	int		i;
	int		k;
	char	*cmd;

	i = -1;
	cmd = shell->rl_str;
	tokens = malloc(sizeof(t_cmd));
	tmp = tokens;
	while (cmd[++i])
	{
		k = 0;
		if (is_space(cmd[i]))
			continue;
		else if (cmd[i] == '|')
			tokens->next = create_token("|", pipes);
		else
			tokens->next = create_cmd_token(cmd + i);
		tokens = tokens->next;
		while (tokens->token[k + 1])
			k++;
		i = i + k;
	}
	return (tmp);
}

int	nbr_of_words(t_cmd *tokens)
{
	int	i;

	i = 0;
	while (tokens != NULL)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

void	lst_to_array(t_minishell *shell, t_cmd *tokens)
{
	int	i;

	shell->cmd_split = (char **)malloc(sizeof(char *) * (nbr_of_words(tokens) + 1));
	if (shell->cmd_split == NULL)
		return ;
	i = 0;
	while (tokens != NULL)
	{
		shell->cmd_split[i] = ft_strdup(tokens->token);
		i++;
		tokens = tokens->next;
	}
	shell->cmd_split[i] = 0;
}
