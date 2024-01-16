/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:13:02 by rui               #+#    #+#             */
/*   Updated: 2024/01/16 16:29:58 by rumachad         ###   ########.fr       */
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
		else if (cmd[i] == '>')
		{
			if (cmd[i + 1] && cmd[i + 1] == '>')
				tokens->next = create_token(">>", redout);
			else
				tokens->next = create_token(">", redout);
		}
		else if (cmd[i] == '<')
		{
			if (cmd[i + 1] && cmd[i + 1] == '<')
				tokens->next = create_token("<<", here_doc);
			else
				tokens->next = create_token("<", redin);
		}
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
	while (tokens != NULL && tokens->type != pipes)
	{
		if (tokens->type != ignore)
			i++;
		tokens = tokens->next;
	}
	return (i);
}

int	lst_to_array(t_minishell *shell, t_cmd *tokens)
{
	t_cmd	*tmp;
	int		i;

	tmp = tokens;
	shell->cmd_split = (char **)malloc(sizeof(char *) * (nbr_of_words(tokens) + 1));
	if (shell->cmd_split == NULL)
		return (ft_fprintf(STDERR_FILENO, "Malloc cmd_split error\n"));
	i = 0;
	while (tokens != NULL && tokens->type != pipes)
	{
		if (tokens->type != ignore)
		{
			shell->cmd_split[i] = ft_strdup(tokens->token);
			i++;
		}
		tokens = tokens->next;
	}
	shell->cmd_split[i] = 0;
	free_tokens(tmp);
	return (0);
}
