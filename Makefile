
NAME = minishell
SRC = main.c builtin.c
OBJS = ${SRC:.c=.o}
CFLAGS = -Wall -Wextra -Werror -I include #-fsanitize=address

%.o:	%.c
		cc ${CFLAGS} -c $< -o $@

${NAME}:	${OBJS}
			make -C libft
			ar rcs minishell.a ${OBJS}
			cc ${CFLAGS} minishell.a libft/libft.a -o ${NAME} -lreadline

all: ${NAME}

clean:
		make clean -C libft
		rm -f ${OBJS}

fclean: clean
		rm -f minishell.a minishell libft/libft.a

re: fclean all

.SILENT:
