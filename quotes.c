/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:57:53 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/23 14:59:53 by rumachad         ###   ########.fr       */
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

char	*remove_quotes(char *rl_str)
{
	char	*tmp;
	int		squotes;
	int		dquotes;
	int		i;
	int		k;
	
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(rl_str) - count_quotes(rl_str) + 1));
	if (tmp == NULL)
		return (NULL);
	i = -1;
	k = 0;
	squotes = 0;
	dquotes = 0;
	while (rl_str[++i] != '\0')
	{
		if (rl_str[i] == '"' && !squotes)
		{
			dquotes = !dquotes;
			continue;
		}
		if (rl_str[i] == '\'' && !dquotes)
		{
			squotes = !squotes;
			continue;
		}
		tmp[k] = rl_str[i];
		k++;
	}
	tmp[k] = '\0';
	free(rl_str);
	return (tmp);
}

int cases_quotes(t_minishell *shell)
{
	int	i;
	int	cmd_quotes;
	
	i = 0;
	cmd_quotes = handle_quotes(shell->cmd);
	if (cmd_quotes && (ft_strchr(shell->split_args, '"') || ft_strchr(shell->split_args, '\'')))
	{
		shell->rl_str = remove_quotes(shell->rl_str);
		return (1);
	}
	else
	{
		while (shell->rl_str[i])
		{
			if (shell->rl_str[i] == '"')
			{
				shell->rl_str = remove_quotes(shell->rl_str);
				break ;
			}
			else if (shell->rl_str[i] == '\'')
			{
				shell->rl_str = remove_quotes(shell->rl_str);
				break ;
			}
			i++;
		}
	}
	return (0);
}
