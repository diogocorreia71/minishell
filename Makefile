
NAME = minishell
SRC = main.c
OBJS = ${SRC:.c=.o}
CFLAGS = -I include #-fsanitize=address #-Wall -Wextra -Werror

%.o:	%.c
		cc ${CFLAGS} -c $< -o $@

${NAME}:	${OBJS}
			make -C libft
			ar rcs minishell.a ${OBJS}
			cc ${CFLAGS} minishell.a libft/libft.a -o ${NAME} -lreadline

all: ${NAME} ./${NAME}

clean:
		make clean -C libft
		rm -f ${OBJS}

fclean: clean
		rm -f minishell.a minishell libft/libft.a

re: fclean all

.SILENT: