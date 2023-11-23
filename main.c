/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/23 15:37:41 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_str(char *rl_str, int len)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = (char *)malloc(sizeof(char) * len + 1);
	if (tmp == NULL)
		return (NULL);
	while (rl_str[i] != ' ' && rl_str[i])
	{
		tmp[i] = rl_str[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*args_str(t_minishell *shell)
{
	char	*tmp;
	int		i;
	int		k;

	i = 0;
	while (shell->rl_str[i] != ' ' && shell->rl_str[i])
		i++;
	shell->cmd = cmd_str(shell->rl_str, i);
	if (shell->rl_str[i] == '\0')
		return (NULL);
	tmp = (char *)malloc(sizeof(char) * ft_strlen(shell->rl_str) - i + 1);
	if (tmp == NULL)
		return (NULL);
	i++;
	k = 0;
	while (shell->rl_str[i])
	{
		tmp[k] = shell->rl_str[i];
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
		shell.split_args = args_str(&shell);
		if (handle_quotes(shell.rl_str) == 1)
		{
			printf("Invalid Quotes\n");
			continue;
		}
		if (cases_quotes(&shell) == 1)
		{
			printf("%s: command not found\n", shell.rl_str);
			continue;
		}
		shell.cmd_split = ft_split(shell.rl_str, ' ');
		builtin_cmd(&shell);
		//Leaks split_args
		if (shell.cmd_split[1])
			free(shell.split_args);
		clean_program(&shell);
	}
}
