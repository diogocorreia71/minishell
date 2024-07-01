/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 12:58:53 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 12:39:21 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_stop(char c)
{
	if (ft_isalnum(c) == 0 && c != '_')
		return (YES);
	return (NO);
}

char	*handle_heredoc_ds(t_env *env, char *token)
{
	int		i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '$')
		{
			i = i + expand(&token, i, env);
			continue ;
		}
		i++;
	}
	return (token);
}

char	*tilde_value(t_env *env, char *token)
{
	char	*value;

	value = NULL;
	if (!ft_strncmp(token, "~+", 3))
		value = get_env_val(env, "PWD");
	else if (!ft_strncmp(token, "~-", 3))
		value = get_env_val(env, "OLDPWD");
	else if (!ft_strncmp(token, "~", 2) || !ft_strncmp(token, "~/", 2))
		value = get_env_val(env, "HOME");
	return (value);
}

char	*expand_tilde(t_env *env, char *token)
{
	char	*value;
	char	*new_token;
	char	*hold;

	value = tilde_value(env, token);
	if (value == NULL)
		return (token);
	hold = ft_strchr(token, '/');
	if (hold != NULL)
		new_token = ft_strjoin(value, hold);
	else
		new_token = ft_strdup(value);
	free(value);
	free(token);
	return (new_token);
}
