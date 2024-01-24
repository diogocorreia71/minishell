/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:46:49 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/24 18:06:59 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_val(t_env *env, char *str)
{
	char	*env_value;

	env_value = get_env(env, str);
	if (env_value == NULL)
		return (ft_strdup("\0"));
	return (ft_strdup(env_value));
}

char	*get_var(char *token)
{
	int		i;
	char	*ds;
	
	ds = ft_strchr(token, '$');
	ds++;
	i = 0;
	if (ft_isalpha(ds[i]) == 0)
		return (ft_substr(ds, 0, 1));
	while (ds[i] != ' ' && ds[i] != '"' && ds[i] != '\''
		&& ds[i] != '$' && ds[i])
		i++;
	return (ft_substr(ds, 0, i));
}

char	*replace(char *var, char *val, char *token)
{
	int		i;
	char	*new_tk;

	i = -1;
	new_tk = (char *)malloc(sizeof(char) * (ft_strlen(token)
			- ft_strlen(var) + ft_strlen(val)));
	if (new_tk == NULL)
		return (NULL);
	while (token[++i] != '$')
		new_tk[i] = token[i];
	new_tk[i] = '\0';
	new_tk = ft_strjoin_get(new_tk, val);
	new_tk = ft_strjoin_get(new_tk, token + ft_strlen(var) + i + 1);
	free(token);
	return (new_tk);
}

char	*isolate(t_env *env, char *token)
{
	char	*var;
	char	*val;

	while (ft_strchr(token, '$'))
	{
		//ex: $HOME, var = HOME
		var = get_var(token);
		val = get_env_val(env, var);
		if (val == NULL)
			val = ft_strdup("\0");
		token = replace(var, val, token);
		free(var);
		free(val);
	}
	return (token);
}

char	*expand_ds(t_env *env, char *token)
{
	char	*new_expanded_token;
	
	new_expanded_token = token;
	if (ft_strchr(token, '~'))
		new_expanded_token = expand_tilde(env, token);
	else if (ft_strchr(token, '$'))
		new_expanded_token = isolate(env, token);
	return (new_expanded_token);
}

t_lst_tokens	*expansion(t_minishell *shell, t_lst_tokens *args)
{	
	t_lst_tokens	*head;
	char			*prev_token;

	prev_token = args->token;
	head = args;
	while (args)
	{
		if (!is_inside_squote(args->token) && ft_strncmp(prev_token, "<<", 3))
			args->token = expand_ds(shell->env, args->token);
		if (!ft_strncmp(args->token, "\0", 1))
			args->type = IGNORE;
		prev_token = args->token;
		args = args->next;
	}
	return (head);
}
