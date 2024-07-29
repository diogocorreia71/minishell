/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:31:44 by rui               #+#    #+#             */
/*   Updated: 2024/07/29 18:10:09 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *cmd)
{
	if (cmd[0] == '-')
	{
		ft_fprintf(STDERR_FILENO, "Invalid Option\n");
		return (1);
	}
	return (0);
}

int	space_input(char *str)
{
	int	i;

	i = 0;
	while (is_space(str[i]) && str[i])
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == 9)
		return (YES);
	else
		return (NO);
}

void	close_fd(int pipe_fd[2])
{
	check_close(close(pipe_fd[0]));
	check_close(close(pipe_fd[1]));
}

int	check_input(char *input, t_env *env)
{
	if (input == NULL)
	{
		free_env(env);
		printf("exit\n");
		exit(g_exit_status);
	}
	else if (ft_strlen(input) == 0 || space_input(input))
		return (1);
	else if (unclosed_quotes(input) == YES)
		return (1);
	return (0);
}
