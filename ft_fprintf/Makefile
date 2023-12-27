# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 16:45:14 by rumachad          #+#    #+#              #
#    Updated: 2023/12/27 09:39:27 by rumachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
SRC = src/ft_fprintf.c src/ft_fprinthexa_fd.c \
		src/ft_fputchar_fd.c src/ft_fputnbr_fd.c src/ft_fputstr_fd.c \
		src/ft_fprinthexa_upp_fd.c src/ft_fput_unsi_nbr_fd.c \
		src/ft_fprintpointer_fd.c
OBJS = ${SRC:.c=.o}
CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra
.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
			ar rcs ${NAME} ${OBJS}

all:	${NAME}

clean:
		${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}

re:	fclean ${NAME}
