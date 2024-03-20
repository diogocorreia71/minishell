/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:32:42 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/20 10:42:35 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gen	*create_heredoc_ptr(t_gen *cmd_ptr, t_env *env,
	t_lst_tokens *head, char *delimiter)
{
	cmd_ptr = redir_constructor(cmd_ptr, 0, O_RDONLY, "hereDoc");
	cmd_ptr = heredoc_constructor(cmd_ptr, ft_strdup(delimiter),
			O_WRONLY | O_CREAT | O_TRUNC);
	init_heredoc((t_heredoc *)cmd_ptr, env, head);
	return (cmd_ptr);
}

t_gen	*create_redir_ptr(t_id redir_type, t_gen *cmd_ptr, char *file)
{
	if (redir_type == REDIR_OUT)
		cmd_ptr = redir_constructor(cmd_ptr, 1,
				O_WRONLY | O_CREAT | O_TRUNC, file);
	else if (redir_type == REDIR_IN)
		cmd_ptr = redir_constructor(cmd_ptr, 0, O_RDONLY, file);
	else if (redir_type == APPEND)
		cmd_ptr = redir_constructor(cmd_ptr, 1,
				O_WRONLY | O_CREAT | O_APPEND, file);
	return (cmd_ptr);
}

t_gen	*parse_redir(t_lst_tokens **args, t_gen	*cmd_ptr,
	t_env *env, t_lst_tokens *head)
{
	t_id	redir_type;
	t_id	token_type;

	redir_type = get_redir_type((*args)->token, (*args)->type);
	if (redir_type == REDIR)
		return (cmd_ptr);
	(*args) = (*args)->next;
	token_type = get_token_type((*args));
	if (token_type != WORD && token_type != EXPAND)
		return (print_syntax_error((*args), cmd_ptr));
	if (redir_type != HERE_DOC)
	{
		if ((*args)->type == EXPAND)
			(*args)->token = expand_token(env, (*args)->token, handle_ds);
		(*args)->token = remove_quotes((*args)->token);
	}
	if (redir_type == HERE_DOC)
		cmd_ptr = create_heredoc_ptr(cmd_ptr, env, head, (*args)->token);
	else
		cmd_ptr = create_redir_ptr(redir_type, cmd_ptr, (*args)->token);
	(*args) = (*args)->next;
	return (cmd_ptr);
}

t_gen	*parser_exec(t_env *env, t_lst_tokens **args)
{
	t_gen			*cmd_ptr;
	t_lst_tokens	*head;
	int				nbr_args;
	t_exec			*exec_cast;

	head = (*args);
	cmd_ptr = exec_constructor();
	exec_cast = (t_exec *)cmd_ptr;
	nbr_args = 0;
	while ((*args) && (*args)->type != PIPE)
	{
		if ((*args)->type == REDIR)
		{
			cmd_ptr = parse_redir(args, cmd_ptr, env, head);
			if (cmd_ptr == NULL)
				return (NULL);
			continue ;
		}
		else if (prepare_token(args, env) == 0)
			nbr_args++;
		(*args) = (*args)->next;
	}
	exec_cast->argv = fill_argv(head, nbr_args);
	return (cmd_ptr);
}

t_gen	*parser_pipe(t_env *env, t_lst_tokens **args)
{
	t_gen	*cmd_ptr;
	t_id	token_type;

	if ((*args)->type == PIPE)
		return (print_syntax_error((*args), NULL));
	cmd_ptr = parser_exec(env, args);
	if (cmd_ptr && get_token_type((*args)) == PIPE)
	{
		(*args) = (*args)->next;
		token_type = get_token_type((*args));
		if (token_type != WORD && token_type != REDIR && token_type != EXPAND)
			return (print_syntax_error((*args), cmd_ptr));
		cmd_ptr = pipe_constructor(cmd_ptr, parser_pipe(env, args));
		if (((t_pipe *)cmd_ptr)->right == NULL)
			return (free_tree(cmd_ptr), NULL);
	}
	return (cmd_ptr);
}
