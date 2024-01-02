/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:30:05 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/02 16:12:59 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_var_name(char *arg)
{
	char	**var_name;
	int		i;

	var_name = (char **)malloc(sizeof(char *) * 2);
	if (var_name == NULL)
		return (NULL);
	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	var_name[0] = ft_substr(arg, 0, i);
	var_name[1] = 0;
	return (var_name);
}

int	check_export(char *cmd)
{
	int	i;

	i = 0;
	if (ft_isdigit(cmd[i]) == 1 || cmd[i] == '=')
		return (1);
	while (cmd[i] != '\0' && cmd[i] != '=')
	{
		if (ft_isalnum(cmd[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	export_syntax(char *cmd)
{
	if (check_export(cmd) == 1)
	{
		ft_fprintf(2, "export: `%s': not a valid identifier\n", cmd);
		return (1);
	}
	return (0);
}

void	create_inv_var(t_env *env, char *var_name)
{
	char	*str;

	str = NULL;
	if (get_env(env, var_name))
		return ;
	else
	{
		env = env_last(env);
		env->next = create_node_env(var_name, "=\0", 0);
	}
}

void	create_vis_var(t_env *env, char **var_name, char *val)
{
	t_env	*tmp;

	tmp = get_env_node(env, *var_name);
	if (tmp)
	{
		free(tmp->var_value);
		tmp->var_value = ft_strdup(ft_strchr(val, '=') + 1);
		return;
	}
	env = env_last(env);
	env->next = create_node_env(*var_name, val, 1);
}

void	export(t_env *env, char **cmd_split)
{
	char	**var_name;
	int		i;

	if (cmd_split[1] == NULL)
	{
		//Fazer o Sort
		sort_env(env);
		return ;
	}
	i = 0;
	while (cmd_split[++i])
	{
		if (export_syntax(cmd_split[i]) == 1)
			continue;
		var_name = get_var_name(cmd_split[i]);
		if (!ft_strchr(cmd_split[i], '='))
			create_inv_var(env, *var_name);
		else
			create_vis_var(env, var_name, cmd_split[i]);
		ft_free_dp((void **)var_name);
	}
}
