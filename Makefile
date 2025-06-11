##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Main Makefile
##

all: zappy_server

zappy_server:
	$(MAKE) -C server

zappy_gui:
	$(MAKE) -C gui

zappy_ai:
	$(MAKE) -C ai

clean:
	$(MAKE) -C server clean

fclean:
	$(MAKE) -C server fclean

re: fclean all

.PHONY: all clean fclean re zappy_server zappy_gui zappy_ai