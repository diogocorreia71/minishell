/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/23 16:52:32 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexer_parser(t_minishell *shell, t_lst_tokens **args)
{
	t_lst_tokens	*tmp;
	
	*args = NULL;
	add_history(shell->rl_str);
	if (handle_quotes(shell->rl_str) == 1)
	{
		ft_fprintf(STDERR_FILENO, "Invalid Quotes\n");
		free(shell->rl_str);
		return (1);
	}
	*args = make_tokens(shell, *args);
	free_first(args);
	free(shell->rl_str);
	expansion(shell, *args);
	tmp = *args;
	while (tmp)
	{
		if (count_quotes(tmp->token) != 0)
			tmp->token = remove_quotes(tmp->token);
		tmp = tmp->next;
	}
	return (0);
}

int main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;
	t_generic		*cmd;

	//Leaks REDIR | EXEC
	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	if (ac != 1 && av)
		return (0);
	shell.env = lst_env(envp);
	while (1)
	{
		// 1.Read Command
		shell.rl_str = readline("minishell$ ");
		if (ft_strlen(shell.rl_str) == 0)
			continue;
		if (lexer_parser(&shell, &args) == 1)
			continue;
		cmd = parser_tokens(&args, &shell.state);
		if (cmd)
		{
			executer_cmd(&shell, cmd);
			if (shell.state == PIPED)
				free_tree(cmd);
		}
	}
}
