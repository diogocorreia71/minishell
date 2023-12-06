/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/06 14:47:08 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	parser(t_minishell *shell)
{
	t_cmd	*args;
	t_cmd	*tmp;
	
	args = NULL;
	if (handle_quotes(shell->rl_str) == 1)
		return (printf("Invalid Quotes\n"), 1);

	// 2.Tokenization 3.Command Identification
	args = make_tokens(shell, args);
	free_first(&args);
	
	// 4.Command Expandsion ($)
	expansion(shell, args);
	
	// 5.Quote removal
	tmp = args;
	while (tmp)
	{
		tmp->token = remove_quotes(tmp);
		tmp = tmp->next;
	}
	// 6.Redirections (>, <)

	// 7.Preparing command execution
	lst_to_array(shell, args);
	free_tokens(args);
	return (0);
}

int main(int ac, char **av, char **envp)
{
	t_minishell	shell;

	if (ac != 1 && av)
		return (0);
	shell.env_array = envp;
	shell.env = dup_env(envp);
	while (1)
	{
		// 1.Read Command
		shell.rl_str = readline("minishell$ ");
		if (ft_strlen(shell.rl_str) == 0)
			continue;
		add_history(shell.rl_str);
		if (parser(&shell) == 1)
			continue;
		builtin_cmd(&shell);
		ft_free_dp((void **)(shell.cmd_split));
		//Leaks
	}
	clean_program(&shell);
}
