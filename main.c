/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/24 18:10:11 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ds_expand(char *token, t_env *env)
{
	char	*expanded_token;
	char	*var;
	int		i;

	i = 0;
	token++;
	expanded_token = NULL;
	while (*token)
	{
		while (*token && *token != '$')
		{
			token++;
			i++;
		}
		var = ft_substr(token, 0, i);
		if (expanded_token == NULL)
			expanded_token = ft_strjoin_get(get_env_val(env, var), token);
		expanded_token = ft_strjoin_get(expanded_token, token);
		free(var);
	}
	return (expanded_token);
}

int	lexer_parser(t_minishell *shell, t_lst_tokens **args)
{
	t_generic		*cmd;
	
	*args = NULL;
	add_history(shell->rl_str);
	if (handle_quotes(shell->rl_str) == 1)
	{
		ft_fprintf(STDERR_FILENO, "Invalid Quotes\n");
		free(shell->rl_str);
		return (1);
	}
	*args = make_tokens(shell, *args);
	free_first(args);
	/* while ((*args))
	{
		printf("%s\n", (*args)->token);
		printf("%d\n", (*args)->type);
		(*args) = (*args)->next;
	}
	return (1); */
	free(shell->rl_str);
	cmd = parser_tokens(shell->env, args);
	/* *args = expansion(shell, *args);
	tmp = *args;
	while (tmp)
	{
		if (count_quotes(tmp->token) != 0)
			tmp->token = remove_quotes(tmp->token);
		tmp = tmp->next;
	} */
	return (1);
	return (0);
}

int main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;

	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	if (ac != 1 && av)
		return (0);
	shell.env = lst_env(envp);
	while (1)
	{
		// 1.Read Command
		shell.rl_str = readline("minishell$ ");
		if (ft_strlen(shell.rl_str) == 0)
			continue;
		if (lexer_parser(&shell, &args) == 1)
			continue;
		/* if (cmd)
		{
			executer_cmd(&shell, cmd);
			free_tree(cmd);
		} */
	}
	return (0);
}
