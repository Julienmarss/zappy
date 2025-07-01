/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** take_object
*/

#ifndef TAKE_OBJECT_HPP
#define TAKE_OBJECT_HPP

#include <vector>
#include <string>
#include <algorithm>  
#include "IA.hpp"
#include "mouv.hpp"
#include <iostream>     
#include <sys/socket.h>    
    

void take_food(IA& ia);
void take_linemate(IA& ia);
void take_deraumere(IA& ia);
void take_sibur(IA& ia);
void take_mendiane(IA& ia);
void take_phiras(IA& ia);
void take_thystame(IA& ia);
void take_action(IA& ia, const std::vector<std::string>& tab);
void handle_movement(int socket, IA& ia);

#endif // TAKE_OBJECT_HPP
