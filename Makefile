# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 12:39:52 by diodos-s          #+#    #+#              #
#    Updated: 2023/12/06 15:58:48 by rumachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_BUILTIN = builtin
SRC_ENV = env
SRC_MINI = minishell

SRC = 	$(wildcard $(SRC_BUILTIN)/*.c) \
		$(wildcard $(SRC_ENV)/*.c) \
		$(wildcard *.c) \
		
OBJS = ${SRC:.c=.o}
CFLAGS = -Wall -Wextra -Werror -I include -fsanitize=address

${NAME}:	${OBJS}
			make -C libft
			ar rcs libminishell.a $(OBJS)
			cc ${CFLAGS} -o $@ -Lminishell -lminishell.a -Llibtf -lft -lreadline

%.o: %.c
	cc ${CFLAGS} -c -o $@ $<


all: ${NAME}

clean:
		make clean -C libft
		rm -f $(OBJS)

fclean: clean
		rm -f libminishell.a minishell libft/libft.a

re: fclean all

val:
		make
		valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

.SILENT:
