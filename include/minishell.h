
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

typedef struct s_minishell
{
    char    *cmd_str;
    char    **cmd_split;
    char    **env;
}               t_minishell;

void	builtin_cmd(t_minishell *cmds);

//free
void	clean_program(t_minishell *cmds);

#endif
