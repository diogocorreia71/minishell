/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 12:58:53 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/24 15:55:22 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*nwtk_tilde(char *val)
{
	char	*new_tk;
	int		i;

	i = -1;
	new_tk = (char *)malloc(sizeof(char) * (ft_strlen(val) + 1));
	if (new_tk == NULL)
		return (NULL);
	while (val[++i])
		new_tk[i] = val[i];
	new_tk[i] = '\0';
	free(val);
	return (new_tk);
}

char	*expand_tilde(t_env *env, char *token)
{
	char	*val;
	char	*new_token;

	val = NULL;
	if (!ft_strncmp(token, "~+", 3))
		val = get_env_val(env, "PWD");
	else if (!ft_strncmp(token, "~-", 3))
		val = get_env_val(env, "OLDPWD");
	else if (!ft_strncmp(token, "~", 2))
		val = get_env_val(env, "HOME");
	else if (!ft_strncmp(token, "~/", 2))
	{
		val = get_env_val(env, "HOME");
		if (val == NULL)
			return (ft_strdup("\0"));
		new_token = ft_strjoin(val, ft_strchr(token, '/'));
		free(val);
		free(token);
		return (new_token);
	}
	if (val == NULL)
		return (ft_strdup("\0"));
	free(token);
	token = nwtk_tilde(val);
	return (token);
}
