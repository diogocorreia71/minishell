/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:09:39 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/31 18:31:39 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	before_readline_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	after_readline_handler(int signum)
{
	if (signum == SIGINT)
		printf("\n");
	else if (signum == SIGQUIT)
		printf("Quit\n");
}

void	init_signals(t_id handler_type)
{
	struct sigaction	sig;

	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	if (handler_type == SIGMAIN)
	{
		sig.sa_handler = before_readline_handler;
		sigaction(SIGINT, &sig, NULL);
		sig.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sig, NULL);
	}
	else if (handler_type == SIGAFR)
	{
		sig.sa_handler = after_readline_handler;
		sigaction(SIGINT, &sig, NULL);
		sigaction(SIGQUIT, &sig, NULL);
	}
	/* else if (handler_type == SIGCHILD)
	{
		sig.sa_handler = child_handler;
		sigaction(SIGINT, &sig, NULL);
		sigaction(SIGQUIT, &sig, NULL);
	} */
}
