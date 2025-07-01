##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Main Makefile
##

all: zappy_server zappy_ai zappy_gui

zappy_server:
	$(MAKE) -C server

zappy_gui:
	$(MAKE) -C gui

zappy_ai:
	$(MAKE) -C ai

clean:
	$(MAKE) -C server clean
	rm -f *.gcda *.gcno

fclean:
	$(MAKE) -C server fclean
	$(MAKE) -C gui fclean
	$(MAKE) -C ai fclean
	rm -f unit_tests *.gcda *.gcno

re: fclean all

tests_run:
	gcc -o unit_tests \
		server/src/server/server.c \
		server/src/server/server_run.c \
		server/src/server/server_accept.c \
		server/src/server/server_events.c \
		server/src/game/game.c \
		server/src/game/game_teams.c \
		server/src/game/game_update.c \
		server/src/game/game_create.c \
		server/src/game/game_resources.c \
		server/src/game/player.c \
		server/src/network/network.c \
		server/src/network/network_handler.c \
		server/src/network/network_client.c \
		server/src/network/network_player.c \
		server/src/network/network_gui.c \
		server/src/network/network_egg.c \
		server/src/network/network_commands.c \
		server/src/parser/args_parser.c \
		server/src/parser/args_teams.c \
		server/src/commands/commands_move.c \
		server/src/commands/commands_info.c \
		server/src/commands/commands_take.c \
		server/src/commands/commands_set.c \
		server/src/commands/commands_utils.c \
		server/src/commands/commands_fork.c \
		server/src/commands/commands_eject.c \
		server/src/commands/commands_eject_utils.c \
		server/src/commands/commands_incantation.c \
		server/src/commands/commands_broadcast.c \
		server/src/commands/commands_look.c \
		server/src/commands/vision/vision_builder.c \
		server/src/commands/vision/vision_coordinates.c \
		server/src/commands/vision/vision_formatter.c \
		server/src/commands/vision/vision_pattern.c \
		server/src/commands/vision/vision_tile_content.c \
		server/src/commands/vision/vision_utils.c \
		server/src/commands/incantation/incantation_validation.c \
		server/src/commands/incantation/incantation_requirements.c \
		server/src/commands/incantation/incantation_execution.c \
		server/src/commands/incantation/incantation_game_end.c \
		server/src/gui/gui_protocol.c \
		server/src/gui/gui_map_commands.c \
		server/src/gui/gui_team_commands.c \
		server/src/gui/gui_player_commands.c \
		server/src/gui/gui_time_commands.c \
		server/src/gui/gui_utils.c \
		server/src/gui/broadcasts/gui_broadcast_player.c \
		server/src/gui/broadcasts/gui_broadcast_action.c \
		server/src/gui/broadcasts/gui_broadcast_resource.c \
		server/src/gui/broadcasts/gui_broadcast_egg.c \
		server/src/gui/broadcasts/gui_broadcast_incantation.c \
		server/src/gui/broadcasts/gui_broadcast_game.c \
		server/src/utils/utils.c \
		server/src/utils/str_utils.c \
		tests/test_commands.c \
		tests/test_game.c \
		tests/test_gui.c \
		tests/test_network.c \
		tests/test_parser.c \
		tests/test_server.c \
		-I./server/include \
		--coverage \
		-lcriterion
	./unit_tests

.PHONY: all clean fclean re zappy_server zappy_gui zappy_ai tests_run
