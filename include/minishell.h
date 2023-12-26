/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:44:59 by diodos-s          #+#    #+#             */
/*   Updated: 2023/12/26 16:47:41 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../ft_fprintf/ft_fprintf.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>

typedef enum s_type
{
	words,
	pipes,
	words_ds
}			t_type;

typedef struct s_pipe
{
	pid_t	*pipe_pid;
	int		nbr_pipes;
	int		**fd;
}				t_pipe;

typedef struct s_cmd
{
	char			*token;
	t_type			type;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_env
{
	char			*var;
	char			*var_value;
	struct s_env	*next;
}				t_env;

typedef struct s_minishell
{
	char	*rl_str;
	char	**cmd_split;
	char	**env_array;
	t_env	*env;
}				t_minishell;

void	builtin_cmd(t_minishell *shell);
void	non_builtin(t_minishell *shell);

//env
t_env	*dup_env(char **envp);

//env_utils
char	*get_env(t_env *env, char *var_str);
char	*get_env_val(t_env *env, char *str);
t_env	*get_env_node(t_env *env, char *key);
t_env	*create_node(char *tmp, char *tmp2);
t_env	*env_last(t_env *env);

//free
void	clean_program(t_minishell *shell);
void	free_env(t_env *env);

//Handle quotes
int		cases_quotes(t_minishell *shell);
char	*remove_quotes(char *str);
int		count_quotes(char *rl_str);
int		handle_quotes(char *str);
char	what_quote(char *str);

//Parser
t_cmd	*make_tokens(t_minishell *shell, t_cmd *tokens);
void	free_tokens(t_cmd *tokens);
void	lst_to_array(t_minishell *shell, t_cmd *tokens);

//Pipes
int		start_pipes(t_minishell *shell, t_pipe *info, t_cmd *args);
void	init_fd_pipes(t_pipe *info);
int		count_pipes(t_cmd *args);

//Expansion
void	expansion(t_minishell *shell, t_cmd *args);
void	expand_tilde(t_env *env, char **token);

//Utils
int		is_space(char c);
void	free_first(t_cmd **tokens);

#endif
