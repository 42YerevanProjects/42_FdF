# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/09 18:35:02 by shovsepy          #+#    #+#              #
#    Updated: 2021/07/09 18:35:03 by shovsepy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= fdf

SRCS		= ./src/main.c ./src/ft_read.c ./src/ft_draw.c
OBJS		= $(SRCS:.c=.o)

CC			= gcc
INCLUDES	= -Imlx
FLAGS		= -Wall -Wextra -Werror

RM			= rm -rf

.c.o:
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $(<:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
	@$(MAKE) -C ./mlx
	@$(MAKE) -C ./libft
	@$(CC) $(OBJS) -Lmlx -lmlx -framework OpenGL -framework AppKit ./libft/libft.a -o $(NAME) 
	@$(RM) $(OBJS)

clean:
	@$(MAKE) -C ./libft fclean
	@$(RM) $(OBJS)

fclean:		clean
	@$(RM) $(NAME) 

re:	fclean all

norm:
	@norminette $(SRCS)

.PHONY: all .c.o clean fclean re norm
