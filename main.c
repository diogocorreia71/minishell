/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/26 17:07:13 by rumachad         ###   ########.fr       */
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
		ft_fprintf(2, "Invalid Quotes\n");
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
		tmp->token = remove_quotes(tmp->token);
		tmp = tmp->next;
	}
	/* tmp = *args;
	while (tmp != NULL)
	{
		printf("%s\n", tmp->token);
		tmp = tmp->next;
	}
	return (1); */
	// 6.Redirections (>, <)

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

int	check_syntax(t_cmd *args)
{
	while (args->next != NULL)
		args = args->next;
	if (args->type == pipes)
		return (1);
	return (0);
}

void	executer(t_minishell *shell, t_cmd *args)
{
	t_pipe	info;
	
	info.nbr_pipes = count_pipes(args);
	if (info.nbr_pipes == 0)
	{
		lst_to_array(shell, args);
		free_tokens(args);
		builtin_cmd(shell);
		ft_free_dp((void **)(shell->cmd_split));
	}
	else
	{
		if (check_syntax(args) == 1)
		{
			ft_fprintf(2, "syntax error near unexpected token `|'\n");
			free_tokens(args);
			return ;
		}
		init_fd_pipes(&info);
		start_pipes(shell, &info, args);
		close_wait(&info);
		free_tokens(args);
		free(info.pipe_pid);
		ft_free_dp((void **)info.fd);
	}
}

int main(int ac, char **av, char **envp)
{
	t_minishell	shell;
	t_cmd		*args;

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
		if (parser(&shell, &args) == 1)
			continue;
		executer(&shell, args);
		free(shell.rl_str);
	}
}
