/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/05 17:16:57 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_env_val(t_env *env, char *str)
{
	char	*env_value;

	env_value = get_env(env, str);
	if (env_value == NULL)
		return (NULL);
	return (ft_strdup(env_value));
}

char	*get_var(char *token)
{
	int	i;

	i = 0;
	while (token[i] && token[i] != ' ' && token[i] != '"')
		i++;
	return (ft_substr(token, 0, i));
}

char	*mk_str(char *token)
{
	char	*tmp;
	int		i;
	int		k;

	i = 0;
	while (token[i] != *ft_strchr(token, '$'))
		i++;
	tmp = (char *)malloc(sizeof(char) * (i + 1));
	k = 0;
	while (k < i)
	{
		tmp[k] = token[k];
		k++;
	}
	return (tmp);
}

char	*replace(t_ds *ds, char *token)
{
	char	*tmp;
	int		i;

	i = 0;
	// 1. Alocar memoria 1 de memoria aqui, 
	// 2. depois criar funcao que copie ate ao $ e usar strjoin
	// 3. Copiar ate ao $, depois usar o strjoin para copiar o val, depois repetir
	tmp = mk_str(token);
	while (tmp)
	{
		ft_strjoin(tmp, ds->val[i++]);
	}
}

char	*isolate(t_env *env, char *token)
{
	t_ds	ds;
	char	*tmp;
	char	*var;
	int		i;

	i = 0;
	ft_memset((void *)&ds, 0, sizeof(t_ds));
	ds.val = (char **)malloc(sizeof(char *) * (count_ds(token) + 1));
	tmp = ft_strchr(token, '$');
	while (tmp)
	{
		tmp++;
		var = get_var(tmp);
		ds.len_var = ft_strlen(var) + ds.len_var;
		ds.val[i++] = get_env_val(env, var);
		tmp = ft_strchr(tmp, '$');
		free(var);
	}
	ds.val[i] = 0;
	ds.len_var = ds.len_var + i;
	return (replace(&ds, token));
}

void	expand_ds(t_env *env, t_cmd arg)
{
	char	*dsign;
	char	quote;
	int		i;
	
	i = 0;
	dsign = ft_strchr(arg.token, '$');
	if (dsign && !ft_isprint(*(dsign + 1)))
		return ;
	quote = what_quote(arg.token);
	if (quote == '\'')
		return ;
	else
		dsign = isolate(env, arg.token);
	printf("%s\n", dsign);
}

void	expansion(t_minishell *shell, t_cmd *args)
{	
	t_cmd	*tmp;

	tmp = args;
	while (tmp)
	{
		if (tmp->type == words && ft_strchr(tmp->token, '$'))
			tmp->type = words_ds;
		tmp = tmp->next;
	}
	tmp = args;
	while (tmp)
	{
		if (tmp->type == words_ds)
			expand_ds(shell->env, *tmp);
		tmp = tmp->next;
	}
}

int	parser(t_minishell *shell)
{
	t_cmd	*args;
	t_cmd	*tmp;
	
	args = NULL;
	if (handle_quotes(shell->rl_str) == 1)
		return (printf("Invalid Quotes\n"), 1);

	// 2.Tokenization 3.Command Identification
	args = make_tokens(shell, args);
	free_first(&args);
	
	// 4.Command Expandsion ($)
	expansion(shell, args);
	return (1);
	
	// 5.Quote removal
	tmp = args;
	while (tmp)
	{
		tmp->token = remove_quotes(tmp);
		tmp = tmp->next;
	}
	// 6.Redirections (>, <)

	// 7.Preparing command execution
	lst_to_array(shell, args);
	free_tokens(args);
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
