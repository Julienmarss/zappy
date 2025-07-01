/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** actions
*/

#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "IA.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

std::vector<std::vector<std::string>> look_and_parse(IA& ia);
void take_action(IA& ia, const std::vector<std::string>& items);

#endif // ACTIONS_HPP