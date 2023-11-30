/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/30 15:54:12 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	what_quote(char *str)
{
	int	i;
	int	squotes;
	int	dquotes;

	i = 0;
	dquotes = 0;
	squotes = 0;
	while (str[i] == '\'' || str[i] == '"')
	{
		if (str[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if (str[i] == '"' && !squotes)
			dquotes = !dquotes;
		i++;
	}
	if (!dquotes && squotes)
		return ('\'');
	else if (dquotes && !squotes)
		return ('"');
	return ('\0');
}

int	count_ds(char *token)
{
	int	i;
	int	ds_counter;

	i = 0;
	ds_counter = 0;
	while (token[i])
	{
		if (token[i] == '$')
				ds_counter++;
		i++;
	}
	return (ds_counter);
}

char	**isolate(char *token)
{
	char	**ds_tokens;
	int		i;
	int		k;
	int		tmp;

	i = -1;
	k = 0;
	ds_tokens = (char **)malloc(sizeof(char *) * (count_ds(token) + 1));
	while (token[++i])
	{
		if (token[i] == '$')
		{
			tmp = i;
			while (token[i] && token[i] != ' ')
				i++;
			ds_tokens[k++] = ft_substr(token, tmp, i - tmp);
		}
		if (token[i] == '\0')
			return (ds_tokens[k] = 0, ds_tokens);
	}
	ds_tokens[k] = 0;
	return (ds_tokens);
}

void	get_env_val(t_env *env, char **ds_tokens)
{
	int		i;
	char	*env_value;

	i = -1;
	while (ds_tokens[++i])
	{
		env_value = get_env(env, ds_tokens[i]);
		free(ds_tokens[i]);
		if (env_value == NULL)
		{
			ds_tokens[i] = ft_strdup("(null)");
			continue;
		}
		ds_tokens[i] = ft_strdup(env_value);
	}
}

int	ds_tokens_len(char **ds_tokens)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (ds_tokens[i])
	{
		len = ft_strlen(ds_tokens[i]) + len;
		i++;
	}
	return (len + i);
}

char	*replace(t_cmd *tokens, char **ds_tokens, int len)
{
	char	*new_token;
	int		len2;
	int		i;
	int		j;
	int		l;
	int		k;
	
	len2 = ds_tokens_len(ds_tokens);
	new_token = (char *)malloc(sizeof(char) * (ft_strlen(tokens->token) - len + len2));
	i = 0;
	j = 0;
	l = 0;
	while (tokens->token[l])
	{
		if (tokens->token[l] == '$')
		{
			k = 0;
			if (tokens->token[l + 1] && (tokens->token[l + 1] == ' '
				|| tokens->token[l + 1] == '"' || tokens->token[l + 1] == '\''))
				new_token[j++] = '$';
			else if (ds_tokens[i] && ft_strcmp(ds_tokens[i], "(null)"))
			{
				while (ds_tokens[i][k])
					new_token[j++] = ds_tokens[i][k++];
			}
			i++;
			while (tokens->token[l] != ' ' && tokens->token[l] != '"'
					&& tokens->token[l] != '\'' && tokens->token[l])
				l++;
			if (tokens->token[l] == '\0')
				return (new_token[j] = '\0', new_token);
		}
		new_token[j++] = tokens->token[l++];
	}
	new_token[j] = '\0';
	free(tokens->token);
	return (new_token);
}

void	expand_ds(t_env *env, t_cmd *tokens)
{
	char	*dsign;
	char	quote;
	char	**ds_tokens;
	/* int		len; */
	
	ds_tokens = NULL;
	dsign = ft_strchr(tokens->token, '$');
	if (dsign == NULL)
		return ;
	if (dsign && !ft_isprint(*(dsign + 1)))
		return ;
	quote = what_quote(tokens->token);
	if (quote == '\'')
		return ;
	else
	{
		//ft_memmove overhaul
		ds_tokens = isolate(tokens->token);
		int i = 0;
		while (ds_tokens[i])
			printf("%s\n", ds_tokens[i++]);
		/* len = ds_tokens_len(ds_tokens);
		get_env_val(env, ds_tokens);
		tokens->token = replace(tokens, ds_tokens, len); */
	}
	env->var = env->var;
}

void	expansion(t_minishell *shell , t_cmd *tokens)
{	
	while (tokens)
	{
		if (tokens->type == words)
			expand_ds(shell->env, tokens);
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
	return (1);
	
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
