/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:44:59 by diodos-s          #+#    #+#             */
/*   Updated: 2024/07/01 01:42:14 by rumachad         ###   ########.fr       */
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
# include <sys/types.h>
# include <signal.h>

# define YES 1
# define NO 0

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
	SIGIGNORE,
	SIGCHILD,
	SIGMAIN,
	SIGHERE,
}			t_id;

typedef struct s_generic
{
	t_id	type;
}				t_gen;

typedef struct s_exec
{
	t_id		type;
	char		**argv;
}				t_exec;

typedef struct s_redir
{
	t_id	type;
	t_gen	*last_pointer;
	int		redir_fd;
	char	*filename;
	int		open_flags;
}				t_redir;

typedef struct s_heredoc
{
	t_id	type;
	t_gen	*heredoc_redir;
	t_id	expansion;
	char	*delimiter;
	int		open_flags;
}				t_heredoc;

typedef struct s_pipe
{
	t_id	type;
	t_gen	*left;
	t_gen	*right;
}				t_pipe;

typedef struct s_pipeline
{
	int	pipe_fd[2];
	int	pipe_pid;
}				t_pipeline;

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
	t_id	in_pipe;
	t_env	*env;
	t_gen	*ast_head;
}				t_minishell;

extern int	g_exit_status;

//Builtin and Execve
void			builtin_cmd(t_minishell *shell, t_exec *cmd);
void			ft_execve(t_minishell *shell);
void			change_shlvl(t_env **env);
char			*exec_path(t_minishell *shell);
void			execve_error(t_minishell *shell, char *path);
char			*get_var_name(char *arg);

//Handle quotes
t_id			unclosed_quotes(char *rl_str);
t_id			token_has_quotes(char *token);
char			*remove_quotes(char *str);
int				count_quotes(char *rl_str);
int				handle_quotes(char *str);

//Lexer
t_lst_tokens	*make_tokens(t_minishell *shell, t_lst_tokens *tokens);

//Parser
int				count_quotes(char *rl_str);
t_gen			*parser_pipe(t_env *env, t_lst_tokens **args);

//Constructors
t_gen			*exec_constructor(void);
t_gen			*redir_constructor(t_gen *cmd, int fd, int flags, char *file);
t_gen			*pipe_constructor(t_gen *left, t_gen *right);
t_gen			*heredoc_constructor(t_gen *cmd, char *delimiter, int flags);

//Expansion
char			*expand_tilde(t_env *env, char *token);
char			*expand_token(t_env *env, char *token,
					char *(handle)(t_env *, char *));
char			*handle_ds(t_env *env, char *token);
char			*handle_heredoc_ds(t_env *env, char *token);
int				expand(char **token, int i, t_env *env);
int				expand_stop(char c);

//Executer
t_id			is_builtin(char *command);
void			executer_cmd(t_minishell *shell, t_gen *cmd);
void			run_exec(t_minishell *shell, t_exec *cmd);
void			run_redir(t_minishell *shell, t_redir *cmd);
void			run_pipeline(t_minishell *shell, t_pipe *cmd);
void			wait_proccess(int pipe_pid);

//HereDoc
int				init_heredoc(t_heredoc *here_doc, t_env *env,
					t_lst_tokens *head);
void			init_heredoc_handler(void);
int				check_heredoc_input(char *input, char *delimiter);
int				run_heredoc(t_heredoc *here_doc, t_env *env, int heredoc_fd);

//Utils (get_type)
t_id			get_redir_type(char *token, t_id token_type);
t_id			get_token_type(t_lst_tokens *arg);

//Free
void			free_env(t_env *env);
void			free_tokens(t_lst_tokens **tokens);
void			free_child(t_gen *ast_head, t_env *env);
void			free_tree(t_gen *cmd);
void			free_heredoc(t_lst_tokens *head, t_env *env, t_gen *cmd);
void			free_redir(t_redir *cmd);
void			*print_syntax_error(t_lst_tokens *arg, t_gen *cmd);

//Signal
void			init_signals(t_id handler_type);
void			main_signal_handler(int signum);

//Utils
int				is_space(char c);
void			free_first(t_lst_tokens **tokens);
void			close_fd(int pipe_fd[2]);
int				prepare_token(t_lst_tokens **args, t_env *env);
char			**fill_argv(t_lst_tokens *args, int nbr_args);
int				space_input(char *str);
int				check_option(char *cmd);
void			swap_node(t_env *sorted_env, t_env *tmp);
int				check_input(char *input, t_env *env);

//Sys_calls check
int				check_fd(int fd, char *message);
int				check_fork(int pid);
int				check_close(int value);
int				check_dup(int value);
int				check_wait(int value);
int				check_signal(int value);

#endif
