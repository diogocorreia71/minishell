/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/31 18:31:20 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

t_generic	*lexer_parser(t_minishell *shell, t_lst_tokens **args)
{
	t_generic	*cmd;
	
	*args = NULL;
	add_history(shell->rl_str);
	*args = make_tokens(shell, *args);
	free_first(args);
	free(shell->rl_str);
	/* while ((*args))
	{
		printf("%s\n", (*args)->token);
		if ((*args)->type == EXPAND)
			printf("EXPAND\n");
		(*args) = (*args)->next;
	}
	return (NULL); */
	cmd = parser_tokens(shell->env, args);
	return (cmd);
}

int	check_input(char *input)
{
	if (input == NULL)
	{
		printf("exit\n");
		exit(EXIT_FAILURE);
	}
	else if (ft_strlen(input) == 0)
		return (1);
	else if (unclosed_quotes(input) == YES)
		return (1);
	return (0);
}

int main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;
	t_generic		*cmd;

	if (ac != 1 && av)
		return (0);
	cmd = NULL;
	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	shell.env = lst_env(envp);
	while (1)
	{
		init_signals(SIGMAIN);
		shell.rl_str = readline("minishell$ ");
		if (check_input(shell.rl_str) == 1)
			continue ;
		init_signals(SIGAFR);
		cmd = lexer_parser(&shell, &args);
		if (cmd != NULL)
		{
			executer_cmd(&shell, cmd);
			free_tree(cmd);
		}
	}
	return (0);
}
