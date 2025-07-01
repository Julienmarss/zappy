/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** game
*/

#ifndef GAME_HPP
#define GAME_HPP

#include "IA.hpp"
#include "mouv.hpp"
#include "actions.hpp"
#include "inventory.hpp"
#include "level_handling.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sys/select.h>
#include <sstream>
#include <algorithm>

void main_game(int socket, IA& ia);

#endif // GAME_HPP