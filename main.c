/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 23:05:48 by rumachad         ###   ########.fr       */
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
		if (((t_heredoc *)cmd)->heredoc_redir != NULL)
			executer_cmd(shell, ((t_heredoc *)cmd)->heredoc_redir);
		unlink("hereDoc");
	}
}

void	wait_pipes(int pipe_pid)
{
	int	status;

	status = 0;
	if (check_wait(waitpid(pipe_pid, &status, 0)) == -1)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = WTERMSIG(status) + 128;
	}
	while (wait(0) != -1)
		;
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
	while (1)
	{
		init_signals(SIGMAIN);
		shell.rl_str = readline("minishell$ ");
		if (check_input(shell.rl_str, shell.env) == 1)
			continue ;
		add_history(shell.rl_str);
		cmd = lexer_parser(&shell, &args);
		if (cmd != NULL)
		{
			shell.ast_head = cmd;
			init_signals(PIPE);
			executer_cmd(&shell, cmd);
			free_tree(shell.ast_head);
		}
	}
	return (0);
}
