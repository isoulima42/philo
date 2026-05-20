# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/01 09:58:14 by isoulima          #+#    #+#              #
#    Updated: 2026/05/04 14:26:51 by isoulima         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo

CC		= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		= src/main.c \
		  src/parse.c \
		  src/init.c \
		  src/time.c \
		  src/monitor.c \
		  src/routine.c

OBJS		= $(SRCS:.c=.o)

HEADER		= include/philo.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) -I include -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
