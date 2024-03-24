/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/24 23:23:26 by rumachad         ###   ########.fr       */
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

void	executer_cmd(t_minishell *shell, t_gen *cmd)
{
	if (cmd->type == EXEC)
		run_exec(shell, (t_exec *)cmd);
	else if (cmd->type == REDIR)
		run_redir(shell, (t_redir *)cmd);
	else if (cmd->type == PIPE)
		run_pipeline(shell, (t_pipe *)cmd);
	else if (cmd->type == HERE_DOC)
	{
		printf("%d\n", ((t_heredoc *)cmd)->heredoc_redir->type);
		if (((t_heredoc *)cmd)->heredoc_redir != NULL)
			executer_cmd(shell, ((t_heredoc *)cmd)->heredoc_redir);
		unlink("hereDoc");
	}
}

int	check_input(char *input, t_env *env)
{
	if (input == NULL)
	{
		free_env(env);
		printf("exit\n");
		exit(EXIT_SUCCESS);
	}
	else if (ft_strlen(input) == 0 || space_input(input))
		return (1);
	else if (unclosed_quotes(input) == YES)
		return (1);
	return (0);
}

/* void	print_tree(t_gen *cmd)
{
	if (cmd == NULL)
		return ;
	if (cmd->type == EXEC)
		printf("EXEC\n");
	else if (cmd->type == REDIR)
	{
		printf("REDIR\n");
		print_tree(((t_redir *)cmd)->last_pointer);
	}
	else if (cmd->type == HERE_DOC)
	{
		printf("HEREDOC\n");
		printf("%d\n", ((t_heredoc *)cmd)->heredoc_redir->type);
		print_tree(((t_heredoc *)cmd)->heredoc_redir);
	}
	else if (cmd->type == PIPE)
	{
		printf("PIPE\n");
		print_tree(((t_pipe *)cmd)->left);
		print_tree(((t_pipe *)cmd)->right);
	}
} */

//Erro: cat << $USER $USER > $USER
int	main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;
	t_gen			*cmd;

	if (ac != 1 && av)
		return (0);
	cmd = NULL;
	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	shell.in_pipe = NO;
	shell.env = lst_env(envp);
	while (1)
	{
		init_signals(SIGMAIN);
		shell.rl_str = readline("minishell$ ");
		add_history(shell.rl_str);
		if (check_input(shell.rl_str, shell.env) == 1)
			continue ;
		cmd = lexer_parser(&shell, &args);
		/* print_tree(cmd); */
		if (cmd != NULL)
		{
			shell.ast_head = cmd;
			executer_cmd(&shell, cmd);
			free_tree(shell.ast_head);
		}
	}
	return (0);
}

