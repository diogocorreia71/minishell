/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/07/01 01:42:16 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

t_gen	*parser_tokens(t_env *env, t_lst_tokens **args)
{
	t_gen			*tree_root;
	t_lst_tokens	*tmp;

	tmp = (*args);
	if (tmp == NULL)
		return (NULL);
	tree_root = parser_pipe(env, &tmp);
	free_tokens(args);
	return (tree_root);
}

t_gen	*lexer_parser(t_minishell *shell, t_lst_tokens **args)
{
	t_gen	*cmd;

	*args = NULL;
	*args = make_tokens(shell, *args);
	free_first(args);
	free(shell->rl_str);
	cmd = parser_tokens(shell->env, args);
	return (cmd);
}

void	run_command(t_minishell *shell, t_gen *cmd)
{
	shell->ast_head = cmd;
	init_signals(PIPE);
	executer_cmd(shell, cmd);
	if (shell->ast_head)
		free_tree(shell->ast_head);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;
	t_gen			*cmd;

	if (ac != 1 && av)
		return (0);
	cmd = NULL;
	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	shell.env = lst_env(envp);
	change_shlvl(&shell.env);
	while (1)
	{
		init_signals(SIGMAIN);
		shell.rl_str = readline("minishell$ ");
		if (check_input(shell.rl_str, shell.env) == 1)
			continue ;
		add_history(shell.rl_str);
		cmd = lexer_parser(&shell, &args);
		if (cmd != NULL)
			run_command(&shell, cmd);
	}
	return (0);
}
