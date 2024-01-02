/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libenv.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 10:17:59 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/02 15:43:58 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBENV_H
# define LIBENV_H

typedef struct s_env
{
	char	*var;
	char	*var_value;
	int		visible;		
	struct s_env	*next;
}				t_env;

//env
t_env	*lst_env(char **envp);
char	**array_env(t_env *env);

//env_utils
char	*get_env(t_env *env, char *var_str);
char	*get_env_val(t_env *env, char *str);
t_env	*get_env_node(t_env *env, char *key);
t_env	*create_node_env(char *tmp, char *tmp2, int flag);
t_env	*env_last(t_env *env);
int		env_size(t_env *env);
void	sort_env(t_env *env);

#endif