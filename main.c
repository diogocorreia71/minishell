/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/18 16:10:37 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_redir(t_cmd *args, int *nbr_redir)
{
	while (args != NULL/*  && args->type != pipes */)
	{
		if (args->type == redir)
			(*nbr_redir)++;
		args = args->next;
	}
}

void	chtype(t_cmd *args, int nbr_redir)
{
	while (nbr_redir != 0)
	{
		if (args->type == redir)
		{
			if (ft_strncmp(args->token, ">>", 3) == 0)
				args->type = append;
			else if (ft_strncmp(args->token, ">", 3) == 0)
				args->type = redout;
			else
				args->type = redin;
			nbr_redir--;
		}
		args = args->next;
	}
}

void	find_cmd(t_cmd *args)
{
	if (args->next->next && args->next->next->type == words)
		args->next->next->type = command;
}

int	has_redir(int npipes, int *orig_fd, t_cmd *args)
{
	int		nbr_redir;

	nbr_redir = 0;
	count_redir(args, &nbr_redir);
	if (nbr_redir == 0)
		return (false);
	if (args->type == redir)
		find_cmd(args);
	chtype(args, nbr_redir);
	if (npipes == 0)
	{
		orig_fd[0] = dup(STDIN_FILENO);
		orig_fd[1] = dup(STDOUT_FILENO);
	}
	return (true);
}

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
	free(shell->rl_str);
	// 4.Command Expandsion ($, ~)
	expansion(shell, *args);
	/* while (*args)
	{
		printf("%s\n", (*args)->token);
		printf("%d\n", (*args)->type);
		*args = (*args)->next;
	}
	return (1); */
	// 5.Quote removal
	tmp = *args;
	while (tmp)
	{
		if (count_quotes(tmp->token) != 0)
			tmp->token = remove_quotes(tmp->token);
		tmp = tmp->next;
	}
	tmp = *args;
	if (check_unex_token(*args) == 1)
	{
		free_tokens(*args);
		return (1);
	}
	return (0);
}

void	reset_fd(int *orig_fd)
{
	dup2(orig_fd[0], STDIN_FILENO);
	close(orig_fd[0]);
	dup2(orig_fd[1], STDOUT_FILENO);
	close(orig_fd[1]);
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
		{
			reset_fd(shell.orig_fd);
			shell.redir_flag = 0;
		}
	}
}
