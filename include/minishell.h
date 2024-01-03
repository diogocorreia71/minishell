/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:44:59 by diodos-s          #+#    #+#             */
/*   Updated: 2024/01/03 17:02:45 by rumachad         ###   ########.fr       */
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

typedef enum s_type
{
	words,
	pipes,
	words_ds,
	redir
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

typedef struct s_rdr
{
	int	nbr_redir;
	int	orig_fd;
	int file_fd;
}				t_rdr;

typedef struct s_minishell
{
	char	*rl_str;
	char	**cmd_split;
	char	**env_array;
	char	*path;
	int		redir_flag;
	t_rdr	rr;
	t_env	*env;
	/* t_env	*srt_env; */
}				t_minishell;

//Builtin and Execve
void	builtin_cmd(t_minishell *shell);
int		non_builtin(t_minishell *shell);
void	ft_execve(t_minishell *shell);
int		execve_syntax(char *cmd, t_env *env, char *path);
void	change_shlvl(char **env_array, t_env *env);
char	*exec_path(t_minishell *shell);
void	execve_error(int error);
char	*get_var_name(char *arg);

//Free
void	execve_clean(t_minishell *shell);
void	free_env(t_env *env);
void	free_all_child(t_minishell *shell, t_pipe *info);
void	free_tokens(t_cmd *tokens);
void	free_prev_node(t_cmd **arg);

//Handle quotes
int		cases_quotes(t_minishell *shell);
char	*remove_quotes(char *str);
int		count_quotes(char *rl_str);
int		handle_quotes(char *str);
char	what_quote(char *str);

//Parser
t_cmd	*make_tokens(t_minishell *shell, t_cmd *tokens);
int		lst_to_array(t_minishell *shell, t_cmd *tokens);
int		count_quotes(char *rl_str);

//Pipes
int		start_pipes(t_minishell *shell, t_pipe *info, t_cmd *args);
int		init_fd_pipes(t_pipe *info);
int		count_pipes(t_cmd *args);

//Redirections
int		handle_redir(t_minishell *shell, t_cmd *args);

//Expansion
void	expansion(t_minishell *shell, t_cmd *args);
void	expand_tilde(t_env *env, char **token);

//Executer
void	executer(t_minishell *shell, t_cmd *args);

//Utils
int		is_space(char c);
void	free_first(t_cmd **tokens);

#endif
