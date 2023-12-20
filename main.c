/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/20 16:35:52 by rumachad         ###   ########.fr       */
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
		printf("%d\n", args->token);
		args = args->next;
	}
	return (1); */
	// 4.Command Expandsion ($, ~)
	expansion(shell, args);
	
	// 5.Quote removal
	tmp = args;
	while (tmp)
	{
		tmp->token = remove_quotes(tmp->token);
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

void	close_wait(t_pipe *info)
{
	int	i;
	
	i = 0;
	while (i < info->nbr_pipes)
	{
		close(info->fd[i][0]);
		close(info->fd[i][1]);
		i++;
	}
	i = 0;
	while (i < info->nbr_pipes + 1)
		waitpid(info->pipe_pid[i++], NULL, 0);
}

void	check_pipe(t_minishell *shell)
{
	t_pipe	info;
	
	info.nbr_pipes = count_pipes(shell);
	if (info.nbr_pipes == 0)
	{
		lst_to_array(shell, shell->args);
		builtin_cmd(shell);
		ft_free_dp((void **)(shell->cmd_split));
		free_tokens(shell->args);
	}
	else
	{
		init_fd_pipes(&info);
		start_pipes(shell, &info);
		close_wait(&info);
		ft_free_dp((void **)info.fd);
		free(info.pipe_pid);
		free_tokens(shell->args);
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
		free(shell.rl_str);
	}
}
