/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:38:34 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/16 17:09:26 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_cmd *args)
{
	while (args->type != redin && args->type != redout && args->type != append)
		args = args->next;
	while (args != NULL)
	{
		args->type = ignore;
		args = args->next;
	}
}

int	start_redir(t_minishell *shell, t_cmd *args)
{
	char	*redir_path;
	int		file_fd;

	while (args != NULL)
	{
		if (args->next)
			redir_path = args->next->token;
		if (args->type == redout || args->type == exe_redout)
			file_fd = open(redir_path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		else if (args->type == redin || args->type == exe_redin)
			file_fd = open(redir_path, O_RDONLY);
		if (file_fd == -1)
			return (-1);
	}
}
