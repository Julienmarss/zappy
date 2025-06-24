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
		src/server/server_events.c		\
		src/game/game.c				\
		src/game/game_teams.c			\
		src/game/game_update.c			\
		src/game/game_create.c		\
		src/game/game_resources.c		\
		src/game/player.c			\
		src/network/network.c			\
		src/network/network_handler.c		\
		src/network/network_client.c		\
		src/network/network_player.c		\
		src/network/network_gui.c		\
		src/network/network_commands.c		\
		src/parser/args_parser.c		\
		src/parser/args_teams.c        \
		src/commands/commands_move.c		\
		src/commands/commands_info.c		\
		src/commands/commands_take.c		\
		src/commands/commands_set.c		\
		src/commands/commands_utils.c		\
		src/commands/commands_fork.c		\
		src/commands/commands_eject.c		\
		src/commands/commands_eject_utils.c		\
		src/commands/commands_incantation.c		\
		src/commands/commands_broadcast.c		\
		src/commands/commands_look.c		\
		src/commands/vision_system.c		\
		src/commands/vision_content.c		\
		src/commands/incantation/incantation_validation.c	\
		src/commands/incantation/incantation_requirements.c	\
		src/commands/incantation/incantation_execution.c	\
		src/commands/incantation/incantation_game_end.c	\
		src/gui/gui_protocol.c			\
		src/gui/gui_map_commands.c		\
		src/gui/gui_team_commands.c		\
		src/gui/gui_player_commands.c		\
		src/gui/gui_time_commands.c		\
		src/gui/gui_utils.c			\
		src/gui/broadcasts/gui_broadcast_player.c		\
		src/gui/broadcasts/gui_broadcast_action.c		\
		src/gui/broadcasts/gui_broadcast_resource.c	\
		src/gui/broadcasts/gui_broadcast_egg.c		\
		src/gui/broadcasts/gui_broadcast_incantation.c	\
		src/gui/broadcasts/gui_broadcast_game.c		\
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
