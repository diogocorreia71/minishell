# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 12:39:52 by diodos-s          #+#    #+#              #
#    Updated: 2023/11/29 12:22:52 by rumachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRC = main.c builtin.c exec_cmd.c env.c cd.c pwd.c echo.c export.c unset.c \
		env_utils.c free_mem.c parser.c quotes.c utils.c
OBJS = ${SRC:.c=.o}
CFLAGS = -Wall -Wextra -Werror -I include -fsanitize=address

%.o:	%.c
		cc ${CFLAGS} -c $< -o $@

${NAME}:	${OBJS}
			make -C libft
			ar rcs minishell.a ${OBJS}
			mv ${OBJS} objs
			cc ${CFLAGS} minishell.a libft/libft.a -o ${NAME} -lreadline

all: ${NAME}

clean:
		make clean -C libft
		rm -f objs/*.o

fclean: clean
		rm -f minishell.a minishell libft/libft.a

re: fclean all

val:
		make
		valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

.SILENT:
