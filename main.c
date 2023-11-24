/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/24 17:12:49 by rumachad         ###   ########.fr       */
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

int	split_quotes(char *rl_str)
{
	int		nbr_words;
	int		i;
	int	dquotes;
	int	squotes;

	i = -1;
	nbr_words = 1;
	dquotes = 0;
	squotes = 0;
	while (rl_str[++i])
	{
		if (rl_str[i] == '"' && !squotes)
		{
			dquotes = !dquotes;
			if (!dquotes && rl_str[i + 1] == ' ')
				nbr_words++;
		}
		if (rl_str[i] == '\'' && !dquotes)
		{
			squotes = !squotes;
			if (!squotes && rl_str[i + 1] == ' ')
				nbr_words++;
		}
	}
	return (nbr_words);
}

int	split_cmd(char *rl_str)
{
	static int	dlm;
	int			dquotes;
	int			squotes;

	dquotes = 0;
	squotes = 0;
	while (rl_str[dlm])
	{
		if (rl_str[dlm] == '"' && !squotes)
		{
			dquotes = !dquotes;
			if (!dquotes && rl_str[dlm + 1] == ' ')
				break ;
		}
		if (rl_str[dlm] == '\'' && !dquotes)
		{
			squotes = !squotes;
			if (!squotes && rl_str[dlm + 1] == ' ')
				break ;
		}
		dlm++;
	}
	return (dlm);
}

void	splitamos(t_minishell *shell)
{
	int	i;
	
	shell->cmd_split = (char **)malloc(sizeof(char *) * (split_quotes(shell->rl_str) + 1));
	if (shell->cmd_split == NULL)
		return (NULL);
	//dlm finish
	i = split_cmd(shell->rl_str);
	/* shell->cmd_split[index] = (char *)malloc(sizeof(char) * (i + 1)); */
	printf("%s\n", shell->cmd_split[0]);
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
		splitamos(&shell);
		if (cases_quotes(&shell) == 1)
		{
			printf("%s: command not found\n", shell.rl_str);
			continue;
		}
		/* shell.cmd_split = ft_split(shell.rl_str, ' '); */
		/* builtin_cmd(&shell); */
		//Leaks split_args
		if (shell.cmd_split[1])
			free(shell.split_args);
		clean_program(&shell);
	}
}
