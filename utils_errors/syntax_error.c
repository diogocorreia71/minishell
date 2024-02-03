/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 15:02:28 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/03 12:21:44 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*print_syntax_error(t_lst_tokens *arg, t_gen *cmd)
{
	if (arg != NULL)
		ft_fprintf(STDERR_FILENO,
			"syntax error near unexpected token `%s'\n", arg->token);
	else
		ft_fprintf(STDERR_FILENO,
			"syntax error near unexpected token `newline'\n");
	g_exit_status = 2;
	free_tree(cmd);
	return (NULL);
}
