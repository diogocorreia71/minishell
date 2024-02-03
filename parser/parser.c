/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:32:42 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 03:22:15 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_generic	*create_heredoc_ptr(t_generic *struct_pointer, t_env *env, t_lst_tokens *head, char *delimiter)
{
	struct_pointer = redir_constructor(struct_pointer, 0, O_RDONLY, "hereDoc");
	struct_pointer = heredoc_constructor(struct_pointer, ft_strdup(delimiter),
		O_WRONLY | O_CREAT | O_TRUNC);
	init_heredoc((t_heredoc *)struct_pointer, env, head);
	return (struct_pointer);
}

t_generic	*create_redir_ptr(t_id redir_type, t_generic *struct_pointer, char *filename)
{
	if (redir_type == REDIR_OUT)
		struct_pointer = redir_constructor(struct_pointer, 1,
			O_WRONLY | O_CREAT | O_TRUNC, filename);
	else if (redir_type == REDIR_IN)
		struct_pointer = redir_constructor(struct_pointer, 0, O_RDONLY, filename);
	else if (redir_type == APPEND)
		struct_pointer = redir_constructor(struct_pointer, 1,
			O_WRONLY | O_CREAT | O_APPEND, filename);
	return (struct_pointer);
}

t_generic	*parse_redir(t_lst_tokens **args, t_generic	*struct_pointer, t_env *env, t_lst_tokens *head)
{
	t_id		redir_type;
	t_id		next_token_type;
	
	redir_type = get_redir_type((*args)->token, (*args)->type);
	if (redir_type == REDIR)
		return (struct_pointer);
	next_token_type = get_token_type((*args)->next);
	if (next_token_type != WORD && next_token_type != EXPAND)
		return (print_syntax_error((*args)->next, struct_pointer));
	(*args) = (*args)->next;
	if (redir_type != HERE_DOC && (*args)->type == EXPAND)
	{
		(*args)->token = expand_token(env, (*args)->token);
		(*args)->token = remove_quotes((*args)->token);
	}
	if (redir_type == HERE_DOC)
		struct_pointer = create_heredoc_ptr(struct_pointer, env, head, (*args)->token);
	else
		struct_pointer = create_redir_ptr(redir_type, struct_pointer, (*args)->token);
	(*args) = (*args)->next;
	return (struct_pointer);
}

char *fill_argv(t_lst_tokens *args, t_env *env)
{
	char	*cmd_arg;
	
	if (args->type == EXPAND)
		args->token = expand_token(env, args->token);
	if (args->token[0] == '\0')
		return (0);
	args->token = remove_quotes(args->token);
	cmd_arg = ft_strdup(args->token);
	return (cmd_arg);
}

t_generic	*parser_exec(t_env *env, t_lst_tokens **args)
{
	t_generic	*struct_pointer;
	t_exec		*exec_cast;
	t_lst_tokens	*head;
	int				nbr_args;

	head = (*args);
	nbr_args = count_tokens((*args));
	struct_pointer = exec_constructor(nbr_args);
	nbr_args = 0;
	exec_cast = (t_exec *)struct_pointer;
	while ((*args) && (*args)->type != PIPE)
	{
		if ((*args)->type == REDIR)
		{
			struct_pointer = parse_redir(args, struct_pointer, env, head);
			continue ;
		}
		if (struct_pointer == NULL)
			return (free_tree(struct_pointer), NULL);
		exec_cast->argv[nbr_args++] = fill_argv((*args), env);
		(*args) = (*args)->next;
	}
	exec_cast->argv[nbr_args] = 0;
	return (struct_pointer);
}

t_generic	*parser_pipe(t_env *env, t_lst_tokens **args)
{
	t_generic	*struct_pointer;
	t_id		token_type;
	
	struct_pointer = parser_exec(env, args);
	if (struct_pointer && get_token_type((*args)) == PIPE)
	{
		(*args) = (*args)->next;
		token_type = get_token_type((*args));
		if (token_type != WORD && token_type != REDIR && token_type != EXPAND)
			return (print_syntax_error((*args), struct_pointer));
		struct_pointer = pipe_constructor(struct_pointer, parser_pipe(env, args));
		if (((t_pipe *)struct_pointer)->right == NULL)
			return (free_tree(struct_pointer), NULL);
	}
	return (struct_pointer);
}

t_generic	*parser_tokens(t_env *env, t_lst_tokens **args)
{
	t_generic		*tree_root;
	t_lst_tokens	*tmp;
	
	tmp = (*args);
	if (tmp == NULL)
		return (NULL);
	tree_root = parser_pipe(env, &tmp);
	free_tokens(args);
	return (tree_root);
}
