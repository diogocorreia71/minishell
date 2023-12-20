/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:57:53 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/20 16:39:36 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotes(char *str)
{
	int	i;
	int	squotes;
	int	dquotes;

	i = 0;
	squotes = 0;
	dquotes = 0;
	while (str[i])
	{
		if (str[i] == '"' && !squotes)
			dquotes = !dquotes;
		if (str[i] == '\'' && !dquotes)
			squotes = !squotes;
		i++;
	}
	if (dquotes == 0 && squotes == 0)
		return (0);
	else
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
		if (rl_str[i] == '\'' && !dquotes)
		{
			squotes++;
			nbr_quotes++;
		}
		i++;
	}
	return (nbr_quotes);
}

char	*remove_quotes(char *str)
{
	char	*tmp;
	char	*word;
	int		squotes;
	int		dquotes;
	int		i;
	int		k;
	
	word = ft_strdup(str);
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(str)
		- count_quotes(str) + 1));
	if (tmp == NULL)
		return (NULL);
	free(str);
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
		tmp[k] = word[i];
		k++;
	}
	tmp[k] = '\0';
	free(word);
	return (tmp);
}

char	what_quote(char *str)
{
	int	i;
	int	squotes;
	int	dquotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
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
