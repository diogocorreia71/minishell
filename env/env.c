/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:02:31 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 12:13:22 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_node_env(char *tmp, char *tmp2, int flag)
{
	t_env	*node;
	char	*a;

	node = malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->var = ft_strdup(tmp);
	a = ft_strchr(tmp2, '=');
	if (a)
		node->var_value = ft_strdup(ft_strchr(tmp2, '=') + 1);
	else
		node->var_value = ft_strdup(tmp2);
	node->visible = flag;
	node->next = NULL;
	return (node);
}

t_env	*lst_env(char **envp)
{
	t_env	*env;
	t_env	*head;
	char	**tmp;
	int		i;

	i = 0;
	head = NULL;
	tmp = ft_split(envp[i], '=');
	env = create_node_env(tmp[0], envp[i], 1);
	ft_free_dp((void **)tmp);
	head = env;
	i++;
	while (envp[i])
	{
		tmp = ft_split(envp[i], '=');
		env->next = create_node_env(tmp[0], envp[i], 1);
		ft_free_dp((void **)tmp);
		env = env->next;
		i++;
	}
	return (head);
}

char	**array_env(t_env *env)
{
	char	**env_array;
	char	*tmp;
	int		i;

	env_array = (char **)malloc(sizeof(char *) * (env_size(env) + 1));
	if (env_array == NULL)
		return (NULL);
	i = 0;
	while (env != NULL)
	{
		tmp = ft_strjoin(env->var, "=");
		env_array[i] = ft_strjoin(tmp, env->var_value);
		free(tmp);
		env = env->next;
		i++;
	}
	env_array[i] = 0;
	return (env_array);
}

void	env_print(t_env *env, char **cmd_split)
{
	if (cmd_split[1])
	{
		if (check_option(cmd_split[1]))
			return ;
		ft_fprintf(STDERR_FILENO, "%s: '%s': No such file or directory\n",
			cmd_split[0], cmd_split[1]);
		g_exit_status = 1;
		return ;
	}
	while (env)
	{
		if (env->visible == 1)
		{
			printf("%s=", env->var);
			printf("%s\n", env->var_value);
		}
		env = env->next;
	}
}
