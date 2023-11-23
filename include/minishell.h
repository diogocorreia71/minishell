/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:44:59 by diodos-s          #+#    #+#             */
/*   Updated: 2023/11/23 15:01:04 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>

typedef struct s_env
{
	char		*var;
	char		*var_value;
	struct	s_env *next;
}				t_env;


typedef struct s_minishell
{
	char	*rl_str;
	char	*cmd;
	char	*split_args;
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
t_env	*get_env_node(t_env *env, char *key);
t_env	*create_node(char *tmp, char *tmp2);
t_env	*env_last(t_env *env);

//free
void	clean_program(t_minishell *shell);
void	free_env(t_env *env);

//Handle quotes
int	cases_quotes(t_minishell *shell);
char	*remove_quotes(char *rl_str);
int		count_quotes(char *rl_str);
int		handle_quotes(char *str);

#endif
