/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** mouv
*/

#ifndef MOUV_HPP
#define MOUV_HPP

#include "IA.hpp" 
#include <vector>
#include <string>
#include <sys/socket.h>
#include <iostream>
#include "IA.hpp"
#include <map>        
#include <sstream>   
#include <algorithm>
#include "take_object.hpp"  
#include "level_handling.hpp" 

class IA;

void choose_mov(int socket, IA& ia, const std::vector<std::vector<std::string>>& double_tableau);
std::vector<std::string> get_steps_to_index(int index);

#endif // MOUV_HPP
