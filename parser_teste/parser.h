#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

typedef enum types
{
	word,
	pipe,
	redir,
	redir_in,
	redir_out,
	append,
	EXEC,
	FILENAME,
	DONE
}		types;

typedef struct cmd
{
	types type;
}			cmd;

typedef struct exec
{
	types type;
	char	*args[50];
}			exec;

typedef struct lst_tokens
{
	char *token;
	types	token_type;
	struct lst_tokens	*next;
}		lst_tokens;


typedef struct s_pipe
{
	types	type;
	cmd		*left;
	cmd		*right;
}			t_pipe;

typedef struct s_redir
{
	types	type;
	cmd		*cmd;
	char	*filename;
	int		fd;
	int 	flags;
}			t_redir;

lst_tokens	*tokenizer();
types 		get_token_type(lst_tokens *token);
int			ft_strncmp(char *str1, char *str2, size_t size);
cmd 		*exec_constructor(void);
cmd 		*pipe_constructor(cmd *left, cmd *right);
cmd 		*redir_constructor(cmd *exec_cmd, int fd, int open_flags, char *filename);

#endif