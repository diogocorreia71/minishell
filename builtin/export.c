/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:30:05 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 11:59:48 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		g_exit_status = 1;
		return (1);
	}
	return (0);
}

void	create_inv_var(t_env *env, char *var_name)
{
	if (get_env(env, var_name))
		return ;
	else
	{
		env = env_last(env);
		env->next = create_node_env(var_name, "=\0", 0);
	}
}

void	create_vis_var(t_env *env, char *var_name, char *val)
{
	t_env	*tmp;

	tmp = get_env_node(env, var_name);
	if (tmp)
	{
		free(tmp->var_value);
		tmp->var_value = ft_strdup(ft_strchr(val, '=') + 1);
		return ;
	}
	env = env_last(env);
	env->next = create_node_env(var_name, val, 1);
}

void	export(t_env *env, char **cmd_split)
{
	t_env	*srt_env;
	char	*var_name;
	int		i;

	if (cmd_split[1] == NULL)
	{
		srt_env = dup_env(env);
		sort_env(srt_env);
		no_args_exp(srt_env);
		free_env(srt_env);
		return ;
	}
	i = 0;
	while (cmd_split[++i])
	{
		if (export_syntax(cmd_split[i]) == 1)
			continue ;
		var_name = get_var_name(cmd_split[i]);
		if (!ft_strchr(cmd_split[i], '='))
			create_inv_var(env, var_name);
		else
			create_vis_var(env, var_name, cmd_split[i]);
		free(var_name);
	}
}
