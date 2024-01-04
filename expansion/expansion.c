/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:46:49 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/04 11:52:06 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_val(t_env *env, char *str)
{
	char	*env_value;

	env_value = get_env(env, str);
	if (env_value == NULL)
		return (NULL);
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

void	isolate(t_env *env, char **token)
{
	char	*var;
	char	*val;

	while (ft_strchr(*token, '$'))
	{
		//ex: $HOME, var = HOME
		var = get_var(*token);
		val = get_env_val(env, var);
		if (val == NULL)
			val = ft_strdup("\0");
		*token = replace(var, val, *token);
		free(var);
		free(val);
	}
}


void	expand_ds(t_env *env, t_cmd *arg)
{
	char	*dsign;

	if (what_quote(arg->token) == '\'')
		return ;
	if (ft_strchr(arg->token, '~') && what_quote(arg->token) == '\0')
	{
		expand_tilde(env, &arg->token);
		return ;
	}
	dsign = ft_strchr(arg->token, '$');
	if (!dsign || (dsign && !ft_isprint(*(dsign + 1))))
		return ;
	isolate(env, &arg->token);
}

void	expansion(t_minishell *shell, t_cmd *args)
{	
	t_cmd	*tmp;

	tmp = args;
	while (tmp)
	{
		if (tmp->type == words && (ft_strchr(tmp->token, '$') 
			|| ft_strchr(tmp->token, '~')))
			tmp->type = words_ds;
		tmp = tmp->next;
	}
	while (args)
	{
		if (args->type == words_ds)
			expand_ds(shell->env, args);
		args = args->next;
	}
}
