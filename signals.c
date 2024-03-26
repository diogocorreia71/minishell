/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:09:39 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/26 10:46:35 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_exit_status = 130;
	}
}

void	sigpipe_handler(int signum)
{
	close(STDIN_FILENO);
	(void)signum;
}

void	sig_pipe(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = 130;
		write(STDERR_FILENO, "\n", 1);
	}
}

void	init_sig_pipe(void)
{
	signal(SIGINT, sig_pipe);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signals(t_id handler_type)
{
	if (handler_type == SIGMAIN)
	{
		signal(SIGINT, main_signal_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (handler_type == SIGCHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, sigpipe_handler);
	}
	else if (handler_type == SIGHERE)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (handler_type == SIGIGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (handler_type == PIPE)
		init_sig_pipe();
}
