/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:30:05 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/10 12:35:38 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_env *env, char *cmd);

void	export(t_env *env, char **cmd_split)
{
	char	*var;
	int		k;
	int		i;

	/* check_export(); */
	i = 0;
	while (*cmd_split[i] != '=' && *cmd_split[i])
		i++;
	var = (char *)malloc(sizeof(char) * (i + 1));
	k = -1;
	while (++k < i)
		var[k] = *cmd_split[k];
	var[k] = '\0';
	//SEG_FAULT Quando o argmento: a =0
	if (get_env(env, var))
		unset(env, var);
	env = env_last(env);
	env->next = create_node(var, *cmd_split);
	free(var);
}
