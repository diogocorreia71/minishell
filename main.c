/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/22 17:18:30 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*args_str(char *rl_str)
{
	char	*tmp;
	int		i;
	int		k;

	i = 0;
	while (rl_str[i] != ' ' && rl_str[i]) 
		i++;
	tmp = (char *)malloc(sizeof(char) * ft_strlen(rl_str) - i + 1);
	if (tmp == NULL)
		return (NULL);
	i++;
	k = 0;
	while (rl_str[i])
	{
		tmp[k] = rl_str[i];
		k++;
		i++;
	}
	tmp[k] = '\0';
	return (tmp);
}

int main(int ac, char **av, char **envp)
{
	t_minishell	shell;

	if (ac != 1 && av)
		return (0);
	shell.env_array = envp;
	shell.env = dup_env(envp);
	while (1)
	{
		shell.rl_str = readline("minishell$ ");
		if (ft_strlen(shell.rl_str) == 0)
			continue;
		add_history(shell.rl_str);
		shell.cmd_split = ft_split(shell.rl_str, ' ');
		if (shell.cmd_split[1])
			shell.split_args = args_str(shell.rl_str);
		if (handle_quotes(shell.rl_str) == 1)
		{
			printf("Invalid Quotes\n");
			clean_program(&shell);
			continue;
		}
		cases_quotes(&shell);
		builtin_cmd(&shell);
		clean_program(&shell);
	}
}
