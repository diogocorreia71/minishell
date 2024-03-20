/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:31:18 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/20 16:05:49 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(char **cmd_split)
{
	char	*dir;

	if (cmd_split[1] && check_option(cmd_split[1]))
		return ;
	dir = getcwd(NULL, 0);
	if (!dir)
	{
		ft_fprintf(STDERR_FILENO, "Can't get cwd\n");
		return ;
	}
	printf("%s\n", dir);
	free(dir);
}
