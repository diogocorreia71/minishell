/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/27 18:50:16 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	nbr_of_words(t_cmd *tokens)
{
	int	i;

	i = 0;
	while (tokens != NULL)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

void	lst_to_array(t_minishell *shell, t_cmd *tokens)
{
	int	i;

	shell->cmd_split = (char **)malloc(sizeof(char *) * (nbr_of_words(tokens) + 1));
	if (shell->cmd_split == NULL)
		return ;
	i = 0;
	while (tokens != NULL)
	{
		shell->cmd_split[i] = ft_strdup(tokens->token);
		i++;
		tokens = tokens->next;
	}
	shell->cmd_split[i] = 0;
}

char	*isolate(char *ds_token)
{
	char	*ds;
	char	*dsign_var;
	int		i;
	int		k;

	ds = ft_strchr(ds_token, '$') + 1;
	if (ft_isdigit(*ds))
		return (NULL); //Caso seja $1USER tem de fazer $1
	i = 0;
	while (ds[i] != ' ' && ds[i] != '"' && ds[i])
		i++;
	/* if (i == 0)
		return (NULL); */
	dsign_var = (char *)malloc(sizeof(char) * (i + 1));
	if (dsign_var == NULL)
		return (NULL);
	k = -1;
	while (++k < i)
		dsign_var[k] = ds[k];
	dsign_var[k] = '\0';
	return (dsign_var);
}

char	*replace(char *env_val, char *ds_token, char *dsign_var)
{
	char	*new_token;
	int		i;
	int		j;
	int		k;

	new_token = (char *)malloc(sizeof(char) * (ft_strlen(ds_token)
			- ft_strlen(dsign_var) + ft_strlen(env_val)));
	if (new_token == NULL)
		return (NULL);
	i = -1;
	k = 0;
	j = 0;
	while (ds_token[++i])
	{
		if (ds_token[i] == '$')
		{
			while (*env_val != '\0')
				new_token[k++] = *env_val++;
			i = i + ft_strlen(dsign_var);
		}
		else
			new_token[k++] = ds_token[i];
	}
	new_token[k] = '\0';
	return (new_token);
}

char	*ds_token(t_env *env, char *ds_token)
{
	char	*env_val;
	char	*tmp;
	char	*dsign_var;

	dsign_var = isolate(ds_token);
	env_val = get_env(env, dsign_var);
	if (env_val == NULL)
		return (NULL);
	tmp = ft_strdup(replace(env_val, ds_token, dsign_var));
	free(dsign_var);
	free(ds_token);
	return (tmp);
}

void	expansion(t_minishell *shell , t_cmd *tokens)
{
	char	*dsign;
	
	while (tokens)
	{
		if (tokens->type == words)
		{
			dsign = ft_strchr(tokens->token, '$');
			if (dsign && ft_isprint(*(dsign + 1)))
				tokens->token = ds_token(shell->env, tokens->token);
			if (tokens->token == NULL)
				return ;
		}
		tokens = tokens->next;
	}
}

int	parser(t_minishell *shell)
{
	t_cmd	*tokens;
	t_cmd	*tmp;
	
	tokens = NULL;
	if (handle_quotes(shell->rl_str) == 1)
		return (printf("Invalid Quotes\n"), 1);

	// 2.Tokenization 3.Command Identification
	tokens = make_tokens(shell, tokens);
	free_first(&tokens);
	
	// 4.Command Expandsion ($)
	expansion(shell, tokens);
	
	// 5.Quote removal
	tmp = tokens;
	while (tmp)
	{
		tmp->token = remove_quotes(tmp);
		tmp = tmp->next;
	}
	// 6.Redirections (>, <)

	// 7.Preparing command execution
	lst_to_array(shell, tokens);
	free_tokens(tokens);
	return (0);
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
		// 1.Read Command
		shell.rl_str = readline("minishell$ ");
		if (ft_strlen(shell.rl_str) == 0)
			continue;
		add_history(shell.rl_str);
		if (parser(&shell) == 1)
			continue;
		builtin_cmd(&shell);
		ft_free_dp((void **)(shell.cmd_split));
		//Leaks
	}
	clean_program(&shell);
}
