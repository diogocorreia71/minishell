/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/02/01 16:56:56 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

t_generic	*lexer_parser(t_minishell *shell, t_lst_tokens **args)
{
	t_generic	*cmd;

	*args = NULL;
	add_history(shell->rl_str);
	*args = make_tokens(shell, *args);
	free_first(args);
	free(shell->rl_str);
	cmd = parser_tokens(shell->env, args);
	return (cmd);
}

int	check_input(char *input)
{
	if (input == NULL)
	{
		printf("exit\n");
		exit(EXIT_FAILURE);
	}
	else if (ft_strlen(input) == 0)
		return (1);
	else if (unclosed_quotes(input) == YES)
		return (1);
	return (0);
}

int	count_word_argv(char *argv)
{
	int	i;
	int	squotes;
	int	dquotes;
	int	count_words;

	i = 0;
	dquotes = 0;
	squotes = 0;
	count_words = 0;
	while (argv[i])
	{
		if (argv[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if (argv[1] == '\"' && !squotes)
			dquotes = !dquotes;
		else if (!dquotes && !squotes && argv[i] == ' ')
			count_words++;
		i++;
	}
	return (count_words);
}

char *mk_arg(char *argv, int start, int end)
{
	if (argv[end + 1] == '\0')
		return (ft_substr(argv, start, end - start + 1));
	else
		return (ft_substr(argv, start, end - start));
}

char	*advance(char *argv, char delimiter)
{
	int	i;

	i = 0;
	while (argv[i] != '\'')
		i++;
	ft_substr(argv, )
}

char	**fill_split(char **cmd_split, char *argv)
{
	int	i;
	int	word;

	i = -1;
	word = 0;
	while (argv[++i])
	{
		if (argv[i] == '\'')
			cmd_split[word++] = advance(argv, '\'');
		else if (argv[1] == '\"')
	}
	cmd_split[word] = 0;
	return (cmd_split);
}

char	**expand_argv(char *argv)
{
	int		nbr_words;
	char	**cmd_split;

	nbr_words = count_word_argv(argv);
	cmd_split = (char **)malloc(sizeof(char *) * (nbr_words + 1));
	if (cmd_split == NULL)
		return (NULL);
	cmd_split = fill_split(cmd_split, argv);
	int i;
	i = -1;
	while (cmd_split[++i])
		printf("%s\n", cmd_split[i]);
	return (cmd_split);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;
	t_generic		*cmd;

	if (ac != 1 && av)
		return (0);
	cmd = NULL;
	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	shell.env = lst_env(envp);
	while (1)
	{
		init_signals(SIGMAIN);
		shell.rl_str = readline("minishell$ ");
		if (check_input(shell.rl_str) == 1)
			continue ;
		cmd = lexer_parser(&shell, &args);
		if (cmd != NULL)
		{
			executer_cmd(&shell, cmd);
			free_tree(cmd);
		}
	}
	return (0);
}
