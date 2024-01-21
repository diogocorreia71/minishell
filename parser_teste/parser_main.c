/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 02:23:44 by rui               #+#    #+#             */
/*   Updated: 2024/01/21 22:17:28 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

types get_redir_type(char *token)
{
	types type;

	type = word;
	if (ft_strncmp(token, ">", 2) == 0)
		type = redir_out;
	else if (ft_strncmp(token, "<", 2) == 0)
		type = redir_in;
	else if (ft_strncmp(token, ">>", 3) == 0)
		type = append;
	return (type);
}

cmd *parse_redir(lst_tokens **tokens, cmd *exec_cmd)
{
	types type;
	
	type = get_token_type(*tokens);
	if (type == DONE)
	{
		printf("syntax error");
		return (exec_cmd);
	}
	type = get_redir_type((*tokens)->token);
	*tokens = (*tokens)->next;
	if (get_token_type(*tokens) != word)
	{
		printf("syntax error");
		return (exec_cmd);
	}
	(*tokens)->token_type = FILENAME;
	if (type == redir_out)
		exec_cmd = redir_constructor(exec_cmd, 1, O_WRONLY | O_CREAT | O_TRUNC, (*tokens)->token);
	else if (type == redir_in)
		exec_cmd = redir_constructor(exec_cmd, 0, O_RDONLY, (*tokens)->token);
	else if (type == append)
		exec_cmd = redir_constructor(exec_cmd, 1, O_WRONLY | O_CREAT | O_APPEND, (*tokens)->token);
	return (exec_cmd);
}

cmd *parse_exec(lst_tokens **tokens)
{
	cmd 	*cmd;
	exec	*pointer_cast;
	int 	i;
	types	type;

	cmd = exec_constructor();
	pointer_cast = (exec *)cmd;
	i = 0;
	while (*tokens && (*tokens)->token_type != pipe)
	{
		type = (*tokens)->token_type;
		if (type == redir)
			cmd = parse_redir(tokens, cmd);
		if (get_token_type(*tokens) != FILENAME)
			pointer_cast->args[i++] = (*tokens)->token;
		*tokens = (*tokens)->next;
	}
	return (cmd);
}

cmd *parse_pipe(lst_tokens **tokens)
{
	types	type;
	cmd	*left;
	
	type = get_token_type(*tokens);
	if (type == DONE)
		return (NULL);
	left = parse_exec(tokens);
	if (left && get_token_type(*tokens) == pipe)
	{
		type = get_token_type((*tokens)->next);
		if (type != word)
		{
			printf("syntax error\n");
			return (NULL);
		}
		left = pipe_constructor(left, parse_pipe(&(*tokens)->next));
	}
	return (left);
}

cmd	*parser_command(lst_tokens **tokens)
{
	cmd *cmd;
	
	cmd = parse_pipe(tokens);
	return (cmd);
}

void	run_cmd(cmd *left)
{
	if (left->type == EXEC)
		printf("make exec\n");
	else if (left->type == redir)
	{
		t_redir *redir_cmd = (t_redir *)left;
		printf("make redir\n");
		run_cmd(redir_cmd->cmd);
	}
	else if (left->type == pipe)
	{
		t_pipe *pipe = (t_pipe *)left;
		printf("make pipe\n");
		run_cmd(pipe->left);
		run_cmd(pipe->right);
	}
	else
		printf("%d\n", left->type);
}

int main(void)
{
	lst_tokens *tokens;
	cmd *parsed_cmd;
	
	tokens = tokenizer();
	parsed_cmd = parser_command(&tokens);
	if (parsed_cmd == NULL)
		return (0);
	run_cmd(parsed_cmd);
    return (0);
}
