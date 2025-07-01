/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** inventory
*/

#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include "IA.hpp"
#include <vector>
#include <string>
#include <map>
#include <sstream>

void get_and_update_inventory(IA& ia);
std::vector<std::string> split(const std::string& str, char delimiter);
void increment_global_inventory(IA& ia);

#endif // INVENTORY_HPP