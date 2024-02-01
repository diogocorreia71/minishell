/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:32:42 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/01 15:53:52 by rumachad         ###   ########.fr       */
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
	(*args)->type = IGNORE;
	if (redir_type == HERE_DOC)
		struct_pointer = create_heredoc_ptr(struct_pointer, env, head, (*args)->token);
	else
		struct_pointer = create_redir_ptr(redir_type, struct_pointer, (*args)->token);
	return (struct_pointer);
}

t_generic	*parser_exec(t_env *env, t_lst_tokens **args)
{
	t_generic	*struct_pointer;
	t_exec		*exec_cast;
	t_lst_tokens	*head;

	head = (*args);
	struct_pointer = exec_constructor();
	exec_cast = (t_exec *)struct_pointer;
	while ((*args) && (*args)->type != PIPE)
	{
		if ((*args)->type == REDIR)
			struct_pointer = parse_redir(args, struct_pointer, env, head);
		if (struct_pointer == NULL)
			return (free_tree(struct_pointer), NULL);
		if ((*args)->type != IGNORE)
		{
			if (exec_cast->argv != NULL)
				exec_cast->argv = ft_strjoin_get(exec_cast->argv, " ");
			if ((*args)->type == EXPAND)
				(*args)->token = expand_token(env, (*args)->token);
			exec_cast->argv = ft_strjoin_get(exec_cast->argv, (*args)->token);
		}
		(*args) = (*args)->next;
	}
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
