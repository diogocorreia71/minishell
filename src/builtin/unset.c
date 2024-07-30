/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:29:35 by rumachad          #+#    #+#             */
/*   Updated: 2024/07/30 11:49:26 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unset(char *cmd)
{
	int	i;

	i = 0;
	if (ft_isdigit(cmd[i]) == 1)
		return (1);
	while (cmd[i])
	{
		if (ft_isalnum(cmd[i]) == 0 && cmd[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

int	unset_syntax(char *cmd)
{
	if (check_unset(cmd) == 1)
	{
		ft_fprintf(2, "unset: `%s': not a valid identifier\n", cmd);
		g_exit_status = 1;
		return (1);
	}
	return (0);
}

void	del_var(t_env **head, t_env *elim)
{
	t_env	*tmp;

	tmp = *head;
	if (tmp == elim)
	{
		*head = elim->next;
		free(elim->var);
		free(elim->var_value);
		free(elim);
		return ;
	}
	while (tmp->next && tmp->next != tmp)
		tmp = tmp->next;
	if (tmp->next == NULL)
		return ;
	tmp->next = elim->next;
	free(elim->var);
	free(elim->var_value);
	free(elim);
	elim = NULL;
}

void	unset(t_minishell *shell, char **cmd_split)
{
	t_env	*tmp;
	int		i;

	i = 0;
	if (cmd_split[1] && check_option(cmd_split[1]))
		return ;
	while (cmd_split[++i])
	{
		if (unset_syntax(cmd_split[i]) == 1)
			continue ;
		tmp = get_env_node(shell->env, cmd_split[i]);
		if (tmp == NULL)
			continue ;
		del_var(&shell->env, tmp);
	}
}
