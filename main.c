/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/02 11:21:22 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser(t_minishell *shell, t_cmd **args)
{
	t_cmd	*tmp;
	
	*args = NULL;
	add_history(shell->rl_str);
	if (handle_quotes(shell->rl_str) == 1)
	{
		ft_fprintf(STDERR_FILENO, "Invalid Quotes\n");
		free(shell->rl_str);
		return (1);
	}
	// 2.Tokenization 3.Command Identification
	*args = make_tokens(shell, *args);
	free_first(args);
	// 4.Command Expandsion ($, ~)
	expansion(shell, *args);
	// 5.Quote removal
	tmp = *args;
	while (tmp)
	{
		if (count_quotes(tmp->token) != 0)
			tmp->token = remove_quotes(tmp->token);
		tmp = tmp->next;
	}
	tmp = *args;
	// 6.Redirections (>, <)
	shell->redir_flag = handle_redir(shell, *args);
	return (0);
}

void	reset_fd(t_rdr *rr)
{
	dup2(rr->orig_fd, STDOUT_FILENO);
	close(rr->orig_fd);
}

int main(int ac, char **av, char **envp)
{
	t_minishell	shell;
	t_cmd		*args;

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
		if (parser(&shell, &args) == 1)
			continue;
		executer(&shell, args);
		if (shell.redir_flag == 1)
			reset_fd(&shell.rr);
	}
}
