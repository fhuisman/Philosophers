# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: fhuisman <fhuisman@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/04/12 15:40:23 by fhuisman      #+#    #+#                  #
#    Updated: 2023/06/29 15:19:42 by fhuisman      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc

FLAGS = -Wall -Wextra -Werror -fsanitize=thread

SRC = main.c init.c utils.c printer.c routine.c error.c

OBJS = ${SRC:.c=.o}

HEADERS = philo.h

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${MLXFLAGS} ${FLAGS} -o ${NAME} ${OBJS} ${LIBFT} ${MLX}

%.o: %.c ${HEADERS}
	${CC} -c ${FLAGS} -o $@ $<

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re