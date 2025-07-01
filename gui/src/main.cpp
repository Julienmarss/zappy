/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** main
*/

#include "../include/Application.hpp"

int main(int argc, char* argv[])
{
    Zappy::Application app;
    
    if (!app.initialize(argc, argv)) {
        return 84;
    }
    
    app.run();
    return 0;
}