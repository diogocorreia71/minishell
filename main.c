/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/26 21:31:02 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	nbr_of_words(t_cmd *tokens)
{
	int	i;

	i = 0;
	while (tokens != NULL)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

void	lst_to_array(t_minishell *shell, t_cmd *tokens)
{
	int	i;

	shell->cmd_split = (char **)malloc(sizeof(char *) * (nbr_of_words(tokens) + 1));
	if (shell->cmd_split == NULL)
		return ;
	i = 0;
	while (tokens != NULL)
	{
		shell->cmd_split[i] = ft_strdup(tokens->token);
		i++;
		tokens = tokens->next;
	}
	shell->cmd_split[i] = 0;
}

void	free_tokens(t_cmd *tokens)
{
	t_cmd *tmp;
	
	while (tokens != NULL)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->token);
		free(tmp);
	}
}

int	parser(t_minishell *shell)
{
	t_cmd	*tokens;
	
	tokens = NULL;
	if (handle_quotes(shell->rl_str) == 1)
		return (printf("Invalid Quotes\n"), 1);

	// 2.Tokenization 3.Command Identification
	tokens = make_tokens(shell, tokens);
	free_first(&tokens);
	
	// 4.Command Expandsion ($)
	
	// 5.Quote removal
	tokens->token = remove_quotes(tokens);
	
	// 6.Redirections (>, <)

	// 7.Preparing command execution
	lst_to_array(shell, tokens);
	free_tokens(tokens);
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
		parser(&shell);
		builtin_cmd(&shell);
		//Leaks
	}
	clean_program(&shell);
}
