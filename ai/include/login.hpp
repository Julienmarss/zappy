/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** login
*/

#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
#include "IA.hpp"

int login(const std::string& ip, int port, const std::string& name, int timeout, IA& ia);

#endif // LOGIN_HPP
