/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:57:53 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/25 01:23:20 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(char *str)
{
	int	i;
	int	squotes;
	int	dquotes;
	int	bslash;

	i = 0;
	bslash = 0;
	squotes = 0;
	dquotes = 0;
	while (str[i])
	{
		if (str[i] == '"' && !squotes)
			dquotes = !dquotes;
		else if (str[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if (str[i] == '\\' && !squotes)
			bslash = !bslash;
		i++;
	}
	if (dquotes == 0 && squotes == 0 && bslash == 0)
		return (0);
	return (1);
}

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

int	count_quotes(char *rl_str)
{
	int		i;
	int		dquotes;
	int		squotes;
	int		nbr_quotes;

	i = 0;
	squotes = 0;
	dquotes = 0;
	nbr_quotes = 0;
	while (rl_str[i])
	{
		if (rl_str[i] == '"' && !squotes)
		{
			dquotes++;
			nbr_quotes++;
		}
		else if (rl_str[i] == '\'' && !dquotes)
		{
			squotes++;
			nbr_quotes++;
		}
		i++;
	}
	return (nbr_quotes);
}

t_id	token_has_quotes(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '"' || token[i] == '\'')
			return (YES);
		i++;
	}
	return (NO);
}

char	*remove_quotes(char *token)
{
	char	*new_token;
	char	*word;
	int		squotes;
	int		dquotes;
	int		i;
	int		k;
	
	if (token_has_quotes(token) == NO)
		return (token);
	word = ft_strdup(token);
	new_token = (char *)malloc(sizeof(char) * (ft_strlen(token)
		- count_quotes(token) + 1));
	if (new_token == NULL)
		return (NULL);
	free(token);
	i = -1;
	k = 0;
	squotes = 0;
	dquotes = 0;
	while (word[++i] != '\0')
	{
		if (word[i] == '"' && !squotes)
		{
			dquotes = !dquotes;
			continue;
		}
		if (word[i] == '\'' && !dquotes)
		{
			squotes = !squotes;
			continue;
		}
		new_token[k] = word[i];
		k++;
	}
	new_token[k] = '\0';
	free(word);
	return (new_token);
}

char	is_inside_squote(char *str)
{
	int	i;
	int	squotes;
	int	dquotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
		i++;
	while (str[i] == '\'' || str[i] == '"')
	{
		if (str[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if (str[i] == '"' && !squotes)
			dquotes = !dquotes;
		i++;
	}
	if (!dquotes && squotes)
		return ('\'');
	else if (dquotes && !squotes)
		return ('"');
	return ('\0');
}
