/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:02:31 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/09 15:31:29 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_env(t_env *env, char *var_str)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->var, var_str) == 0)
			return (ft_strdup(env->var_value));
		env = env->next;
	}
	return (NULL);
}

t_env	*create_node(char *tmp, char *tmp2)
{
	t_env	*node;
	int i;

	i = 0;
	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->var = ft_strdup(tmp);
	node->var_value = ft_strdup(ft_strchr(tmp2, '=') + 1);
	node->next = NULL;
	return (node);
}

t_env	*dup_env(char **envp)
{
	t_env	*env;
	t_env	*head;
	char	**tmp;
	int		i;

	i = 0;
	tmp = ft_split(envp[i++], '=');
	env = create_node(tmp[0], envp[i]);
	ft_free_dp((void **)tmp);
	head = NULL;
	head = env;
	while (envp[i])
	{
		tmp = ft_split(envp[i], '=');
		env->next = create_node(tmp[0], envp[i]);
		ft_free_dp((void **)tmp);
		env = env->next;
		i++;
	}
	return (head);
}
