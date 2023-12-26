# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 12:39:52 by diodos-s          #+#    #+#              #
#    Updated: 2023/12/26 16:57:38 by rumachad         ###   ########.fr        #
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
			make -C ft_fprintf
			ar rcs minishell.a $(OBJS)
			cc ${CFLAGS} minishell.a libft/libft.a ft_fprintf/libftprintf.a -lreadline -o $@

%.o: %.c
	cc ${CFLAGS} -c -o $@ $<

all: ${NAME}

clean:
		make clean -C libft
		make clean -C ft_fprintf
		rm -f $(OBJS)

fclean: clean
		rm -f minishell.a minishell libft/libft.a ft_fprintf/libftprintf.a

re: fclean all

val:
		make
		valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

.SILENT:
