/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/20 15:01:09 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	number_quotes(char *cmd_str, int q)
{
	int	i;
	int	qc;

	i = 0;
	qc = 0;
	while (cmd_str[i])
	{
		if (cmd_str[i] == q)
			qc++;
		i++;
	}
	return (qc);
}

char	*remove_quotes(char *cmd_str, int qc)
{
	char	*rem_q;
	int		i;
	int		k;

	rem_q = (char *)malloc(sizeof(char) * (ft_strlen(cmd_str) - qc + 1));
	if (rem_q == NULL)
		return (NULL);
	i = 0;
	k = 0;
	while (cmd_str[i])
	{
		if (cmd_str[i] != 34 && cmd_str[i] != 39)
		{
			rem_q[k] = cmd_str[i];
			k++;
		}
		i++;
	}
	rem_q[k] = '\0';
	free(cmd_str);
	return (rem_q);
}

int	process_quotes(char **cmd_str)
{
	int		i;
	int		qc;
	
	i = 0;
	qc = 0;
	while ((*cmd_str)[i])
	{
		if ((*cmd_str)[i] == 34 || (*cmd_str)[i] == 39)
		{
			if ((*cmd_str)[i] == 34)
				qc = number_quotes(*cmd_str, 34);
			else
				qc = number_quotes(*cmd_str, 39);
			break ;
		}
		i++;
	}
	if ((qc % 2) == 0)
	{
		if (qc != 0)
			*cmd_str = remove_quotes(*cmd_str, qc);
		return (0);
	}
	else
		return (1);
}

int	parser(t_minishell *shell)
{
	//Process quotes ("", '')
	if (process_quotes(&shell->cmd_str) == 1)
	{
		printf("Unclosed Quotes\n");
		return (1);
	}
	//Split the command name and arguments
	shell->cmd_split = ft_split(shell->cmd_str, ' ');
	return (0);
}

int main(int ac, char **av, char **envp)
{
	t_minishell	shell;

	if (ac != 1 && av)
		return (0);
	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	shell.env_array = envp;
	shell.env = dup_env(envp);
	while (1)
	{
		shell.cmd_str = readline("minishell$ ");
		if (ft_strlen(shell.cmd_str) == 0)
			continue;
		add_history(shell.cmd_str);
		if (parser(&shell) == 1)
			continue;
		builtin_cmd(&shell);
		clean_program(&shell);
	}
}
