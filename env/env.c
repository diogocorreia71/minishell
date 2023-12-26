/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:02:31 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/26 15:31:26 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_last(t_env *env);

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

t_env	*create_node(char *tmp, char *tmp2)
{
	t_env	*node;
	
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
	head = NULL;
	tmp = ft_split(envp[i], '=');
	env = create_node(tmp[0], envp[i]);
	ft_free_dp((void **)tmp);
	head = env;
	i++;
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

void	env_print(t_env *env, char **cmd_split)
{
	if (cmd_split[1])
	{
		ft_putstr_fd(cmd_split[0], STDERR_FILENO);
		ft_putstr_fd(": '", STDERR_FILENO);
		ft_putstr_fd(cmd_split[1], STDERR_FILENO);
		ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
		return ;
	}
	while (env)
	{
		printf("%s=", env->var);
		printf("%s\n", env->var_value);
		env = env->next;
	}
}
