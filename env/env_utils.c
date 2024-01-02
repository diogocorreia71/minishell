/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:59:09 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/02 15:47:08 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_node(t_env *env, char *key)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->var, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*get_env(t_env *env, char *var_str)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->var, var_str) == 0)
			return (env->var_value);
		env = env->next;
	}
	return (NULL);
}

int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env != NULL)
	{
		env = env->next;
		i++;
	}
	return (i);
}

t_env	*env_last(t_env *env)
{
	while (env != NULL)
	{
		if (env->next == NULL)
			break ;
		env = env->next;
	}
	return (env);
}
