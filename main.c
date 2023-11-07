/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diodos-s <diodos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/07 15:11:01 by diodos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_program(t_minishell *cmds)
{
	int	i;

	i = 1;
	while (cmds->cmd_split[i])
	{
		free(cmds->cmd_split[i]);
		i++;
	}
	free(cmds->cmd_split);
	free(cmds->cmd_str);
}

int main(int ac, char **av, char **envp)
{
	t_minishell	cmds;

	if (ac != 1 && av)
		return (0);
	cmds.env = envp;
	while (1)
	{
		cmds.cmd_str = readline("minishell$ ");
		if (ft_strlen(cmds.cmd_str) == 0)
			continue;
		add_history(cmds.cmd_str);
		cmds.cmd_split = ft_split(cmds.cmd_str, ' ');
		builtin_cmd(&cmds);
		clean_program(&cmds);
	}
}
