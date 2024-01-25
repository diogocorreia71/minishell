/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:44:59 by diodos-s          #+#    #+#             */
/*   Updated: 2024/01/25 02:17:45 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../include/libenv.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <limits.h>
# include <stdbool.h>

typedef enum s_id
{
	WORD,
	EXPAND,
	PIPE,
	REDIR,
	APPEND,
	REDIR_IN,
	REDIR_OUT,
	HERE_DOC,
	EXEC,
	DONE,
	IGNORE,
	YES,
	NO,
}			t_id;

typedef struct s_generic
{
	t_id	type;
}				t_generic;

typedef struct s_exec
{
	t_id	type;
	char	*argv;
}				t_exec;

typedef struct s_redir
{
	t_id		type;
	t_generic	*last_pointer;
	int			redir_fd;
	char		*filename;
	int			open_flags;
}				t_redir;

typedef struct s_heredoc
{
	t_id		type;
	t_id		expansion;
	t_generic	*heredoc_redir;
	char		*delimiter;
	int			open_flags;
}				t_heredoc;

typedef struct s_pipe
{
	t_id		type;
	t_generic	*left;
	t_generic	*right;
}				t_pipe;

typedef struct s_lst_tokens
{
	char				*token;
	t_id				type;
	struct s_lst_tokens	*next;
}			t_lst_tokens;

typedef struct s_minishell
{
	char	*rl_str;
	char	**cmd_split;
	char	**env_array;
	char	*path;
	t_env	*env;
}				t_minishell;

//Builtin and Execve
void	builtin_cmd(t_minishell *shell, t_exec *cmd);
int		non_builtin(t_minishell *shell);
void	ft_execve(t_minishell *shell);
int		execve_syntax(char *cmd, t_env *env, char *path);
void	change_shlvl(char **env_array, t_env *env);
char	*exec_path(t_minishell *shell);
void	execve_error(int error);
char	*get_var_name(char *arg);


//Handle quotes
t_id	unclosed_quotes(char *rl_str);
t_id	token_has_quotes(char *token);
char	*remove_quotes(char *str);
int		count_quotes(char *rl_str);
char	is_inside_squote(char *str);

//Lexer
t_lst_tokens	*make_tokens(t_minishell *shell, t_lst_tokens *tokens);

//Parser
int				count_quotes(char *rl_str);
t_generic		*parser_tokens(t_env *env, t_lst_tokens **args);
//Constructors
t_generic	*exec_constructor(void);
t_generic	*redir_constructor(t_generic *cmd, int fd, int flags, char *filename);
t_generic	*pipe_constructor(t_generic *left, t_generic *right);
t_generic	*heredoc_constructor(t_generic *cmd, char *delimiter, int flags);

//Expansion
t_lst_tokens	*expansion(t_minishell *shell, t_lst_tokens *args);
char			*get_var(char *token);
char			*expand_tilde(t_env *env, char *token);
char			*expand_ds(t_env *env, char *token);

//Executer
t_id	is_builtin(char *command);
void	executer_cmd(t_minishell *shell, t_generic *cmd);
void	run_exec(t_minishell *shell, t_exec *cmd);
void	run_redir(t_minishell *shell, t_redir *cmd);
void	run_pipe(t_minishell *shell, t_pipe *cmd);

//HereDoc
void	prepare_hereDoc(t_generic *struct_pointer, t_env *env);

//Utils (get_type)
t_id		get_redir_type(char *token, t_id token_type);
t_id	    get_token_type(t_lst_tokens *arg);

//Free
void	free_env(t_env *env);
void	free_tokens(t_lst_tokens **tokens);
void	free_child(t_minishell *shell, t_pipe *cmd);
void	clean_program(t_minishell *shell, t_generic *cmd, t_id mode);
void	free_tree(t_generic *cmd);

//Utils
int		is_space(char c);
void	free_first(t_lst_tokens **tokens);
int		check_unex_token(t_lst_tokens *args);
void	close_fd(int pipe_fd[2]);

#endif
