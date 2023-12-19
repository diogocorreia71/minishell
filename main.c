/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/19 15:16:16 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser(t_minishell *shell)
{
	t_cmd	*args;
	t_cmd	*tmp;
	
	args = NULL;
	add_history(shell->rl_str);
	if (handle_quotes(shell->rl_str) == 1)
		return (printf("Invalid Quotes\n"), 1);

	// 2.Tokenization 3.Command Identification
	args = make_tokens(shell, args);
	free_first(&args);
	/* while (args != NULL)
	{
		printf("%s\n", args->token);
		args = args->next;
	}
	return (1); */
	// 4.Command Expandsion ($, ~)
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
	shell->args = args;
	/* lst_to_array(shell, args);
	int i = 0;
	while (shell->cmd_split[i])
		printf("%s\n", shell->cmd_split[i++]);
	return (1);
	free_tokens(args); */
	return (0);
}

int	count_pipes(t_minishell *shell)
{
	t_cmd	*args;
	int		nbr_pipes;
	
	args = shell->args;
	nbr_pipes = 0;
	while (args != NULL)
	{
		if (args->type == pipes)
			nbr_pipes++;
		args = args->next;
	}
	return (nbr_pipes);
}

int	start_pipes(t_minishell *shell, int nbr_pipes)
{
	int		fd[2];
	pid_t	*pipe_pid;
	int		i;
	t_cmd	*args;

	/* int	i;

	i = 0;
	while (i < nbr_pipes)
	{
		pipe(fd);
		i++;
	} */
	if (pipe(fd) == -1)
		return (1);
	pipe_pid = (pid_t *)malloc(sizeof(pid_t) * (nbr_pipes + 2));
	i = 0;
	args = shell->args;
	while (i < nbr_pipes + 1)
	{
		pipe_pid[i] = fork();
		if (pipe_pid[i] == -1)
			return (2);
		if (pipe_pid[i] == 0)
		{
			lst_to_array(shell, args);
			int i = 0;
			while (shell->cmd_split[i])
				printf("%s\n", shell->cmd_split[i++]);
			exit(0);
		}
		while (args != NULL && args->type != pipes)
			args = args->next;
		/* if (args->type == pipes)
			args = args->next; */
		i++;
		/* printf("%s\n", args->token); */
	}
	return (0);
}

void	check_pipe(t_minishell *shell)
{
	int	nbr_pipes;
	
	nbr_pipes = count_pipes(shell);
	if (nbr_pipes == 0)
	{
		lst_to_array(shell, shell->args);
		free_tokens(shell->args);
	}
	else
	{
		start_pipes(shell, nbr_pipes);
	}
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
		if (parser(&shell) == 1)
			continue;
		check_pipe(&shell);
		return (0);
		builtin_cmd(&shell);
		ft_free_dp((void **)(shell.cmd_split));
		free(shell.rl_str);
	}
}
