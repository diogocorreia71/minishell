# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 12:39:52 by diodos-s          #+#    #+#              #
#    Updated: 2024/03/20 15:29:10 by rumachad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

VPATH = builtin env execution execve expansion lexer parser utils_errors
SRC = main.c \
	parser.c \
	execve_utils.c \
	execve_cmd.c \
	struct_constructors.c \
	parser_utils.c \
	signals.c \
	export_utils.c \
	exit.c \
	pwd.c \
	builtin.c \
	export.c \
	unset.c \
	cd.c \
	echo.c \
	expansion.c \
	expansion2.c \
	env.c \
	env_utils.c \
	lexer.c \
	quotes.c \
	quotes_utils.c \
	heredoc.c \
	run_cmd.c \
	executer.c \
	check_sys.c \
	syntax_error.c \
	utils.c \
	free_mem_lst.c \
	free_mem.c \

OBJS_DIR = obj
OBJS = $(addprefix $(OBJS_DIR)/,$(SRC:.c=.o))
CFLAGS = -Wall -Wextra -Werror -I include #-fsanitize=address

${NAME}:	${OBJS}
			make -C libft
			ar rcs minishell.a $(OBJS)
			cc ${CFLAGS} minishell.a libft/libft.a -lreadline -o $@

$(OBJS_DIR)/%.o: %.c
	mkdir -p $(OBJS_DIR)
	cc ${CFLAGS} -c $< -o $@

all: ${NAME}

clean:
		make clean -C libft
		rm -f $(OBJS)

fclean: clean
		rm -f minishell.a minishell libft/libft.a

re: fclean all

