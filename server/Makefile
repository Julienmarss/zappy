##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Server Makefile
##

NAME	=	../zappy_server

CC	=	gcc

RM	=	rm -f

CFLAGS	=	-Wall -Wextra -Werror
CFLAGS	+=	-I./include

SRCS	=	src/main.c				\
		src/server/server.c			\
		src/server/server_run.c			\
		src/server/server_accept.c		\
		src/game/game.c				\
		src/game/game_teams.c			\
		src/game/game_update.c			\
		src/game/player.c			\
		src/network/network.c			\
		src/network/network_client.c		\
		src/network/network_commands.c		\
		src/commands/commands_move.c		\
		src/commands/commands_info.c		\
		src/commands/commands_action.c		\
		src/gui/gui_protocol.c			\
		src/utils/utils.c			\
		src/utils/str_utils.c

OBJS	=	$(SRCS:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CC) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

debug:	CFLAGS += -g3
debug:	re

.PHONY:	all clean fclean re debug
