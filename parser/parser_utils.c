/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:40:01 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 17:45:52 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_id	get_token_type(t_lst_tokens *arg)
{
	if (arg == NULL)
		return (DONE);
	else
		return (arg->type);
}

t_id	get_redir_type(char *token, t_id token_type)
{
	t_id	type;

	type = token_type;
	if (ft_strncmp(token, ">", 2) == 0)
		type = REDIR_OUT;
	else if (ft_strncmp(token, "<", 2) == 0)
		type = REDIR_IN;
	else if (ft_strncmp(token, ">>", 3) == 0)
		type = APPEND;
	else if (ft_strncmp(token, "<<", 3) == 0)
		type = HERE_DOC;
	return (type);
}

int	prepare_token(t_lst_tokens **args, t_env *env)
{
	if ((*args)->type == EXPAND)
		(*args)->token = expand_token(env, (*args)->token, handle_ds);
	if ((*args)->token[0] == '\0')
		return (-1);
	(*args)->token = remove_quotes((*args)->token);
	(*args)->type = WORD;
	return (0);
}

char	**fill_argv(t_lst_tokens *args, int nbr_args)
{
	char	**argv;
	int		i;

	argv = (char **)ft_calloc(sizeof(char *), (nbr_args + 1));
	i = 0;
	while (args && i < nbr_args)
	{
		if (args->token[0] == '\0')
		{
			args = args->next;
			continue ;
		}
		else if (args->type == REDIR)
		{
			args = args->next;
			if (args)
				args = args->next;
			continue ;
		}
		argv[i++] = ft_strdup(args->token);
		args = args->next;
	}
	argv[i] = 0;
	return (argv);
}
