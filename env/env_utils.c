/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:59:09 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/10 14:48:23 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_node(t_env *env, char *key)
{
	while(env != NULL)
	{
		if (ft_strcmp(env->var, key) == 0)
			break;
		env = env->next;
	}
	return (env);
}

t_env	*env_last(t_env *env)
{
	while(env != NULL)
	{
		if (env->next == NULL)
			break;
		env = env->next;
	}
	return (env);
}
