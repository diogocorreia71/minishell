/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:59:09 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/29 12:19:36 by rumachad         ###   ########.fr       */
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

void	env_update(t_env *env, char **env_array)
{
	char	*str;
	int		i;

	i = 0;
	str = NULL;
	while (env != NULL)
	{
		str = ft_strjoin(env->var, "=");
		str = ft_strjoin_get(str, env->var_value);
		if (ft_strcmp(str, env_array[i]) != 0)
		{
			free(env_array[i]);
			env_array[i] = str;
		}
		i++;
	}
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
