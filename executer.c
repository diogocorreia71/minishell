/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:36:27 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/16 16:10:54 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_unex_token(t_cmd *args)
{
	int	counter;

	counter = 0;
	while (args->next != NULL)
	{
		if (args->type == pipes || args->type == redir)
		{
			counter++;
			if (counter == 2)
				break ;
		}
		else
			counter = 0;
		args = args->next;
	}
	if (args->type == pipes || args->type == redir)
	{
		if (args->type == redir && args->next == NULL)
			ft_fprintf(2, "syntax error near unexpected token `newline'\n");
		else
			ft_fprintf(2, "syntax error near unexpected token `%s'\n", args->token);
		return (1);
	}
	return (0);
}

void	executer(t_minishell *shell, t_cmd *args)
{
	t_pipe	info;
	
	info.nbr_pipes = count_pipes(args);
	if (info.nbr_pipes == 0)
	{
		start_redir(shell, args);
		if (lst_to_array(shell, args) != 0)
			return ;
		builtin_cmd(shell);
		ft_free_dp((void **)(shell->cmd_split));
	}
	else
	{
		if (init_fd_pipes(&info) != 0)
			return ;
		start_pipes(shell, &info, args);
		close_wait(&info);
		ft_free_dp((void **)info.fd);
		free(info.pipe_pid);
	}
}

/* int i = 0;
while (shell->cmd_split[i])
{
	printf("%s\n", shell->cmd_split[i]);
	i++;
}
return ; */