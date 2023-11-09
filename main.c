/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/08 17:01:00 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp->var);
		free(tmp->var_value);
		free(tmp);
	}	
}

void	clean_program(t_minishell *cmds)
{
	int	i;

	i = 0;
	ft_free_dp((void **)cmds->cmd_split);
	free(cmds->cmd_str);
}

int main(int ac, char **av, char **envp)
{
	t_minishell	cmds;

	if (ac != 1 && av)
		return (0);
	cmds.env_array = envp;
	cmds.env = dup_env(envp);
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
