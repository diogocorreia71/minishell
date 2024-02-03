/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:57:53 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 18:56:03 by rumachad         ###   ########.fr       */
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
		i++;
	}
	if (dquotes == 0 && squotes == 0 && bslash == 0)
		return (0);
	return (1);
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

char	*rq_token(char *token, char *new_token)
{
	int		i;
	int		k;
	int		squotes;
	int		dquotes;

	i = -1;
	k = 0;
	squotes = 0;
	dquotes = 0;
	while (token[++i] != '\0')
	{
		if (token[i] == '"' && !squotes)
		{
			dquotes = !dquotes;
			continue ;
		}
		if (token[i] == '\'' && !dquotes)
		{
			squotes = !squotes;
			continue ;
		}
		new_token[k++] = token[i];
	}
	new_token[k] = '\0';
	return (new_token);
}

char	*remove_quotes(char *token)
{
	char	*new_token;

	if (token_has_quotes(token) == NO)
		return (token);
	new_token = (char *)malloc(sizeof(char) * (ft_strlen(token)
				- count_quotes(token) + 1));
	if (new_token == NULL)
		return (NULL);
	new_token = rq_token(token, new_token);
	free(token);
	return (new_token);
}
