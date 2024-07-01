/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 17:48:40 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 23:05:41 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp->var);
		free(tmp->var_value);
		free(tmp);
	}
}

void	free_tokens(t_lst_tokens **args)
{
	t_lst_tokens	*tmp;

	while (*args != NULL)
	{
		tmp = *args;
		*args = (*args)->next;
		free(tmp->token);
		free(tmp);
	}
}

void	free_first(t_lst_tokens **tokens)
{
	t_lst_tokens	*tmp;

	tmp = *tokens;
	*tokens = (*tokens)->next;
	free(tmp);
}
