/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** IA
*/

#include "IA.hpp"

/// @fn IA::IA()
/// @brief Constructeur de la classe IA.
/// Initialise un identifiant aléatoire unique pour ce client IA.
IA::IA()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000000);
    random_num = dis(gen);
}

/// @fn void IA::increment_food()
/// @brief Ajoute une unité de nourriture à l'inventaire de l'IA.
void IA::increment_food()
{
    nb_food++;
}

/// @fn void IA::increment_linemate()
/// @brief Ajoute une unité de linemate à l'inventaire de l'IA.
void IA::increment_linemate()
{ 
    nb_linemate++; 
}

/// @fn void IA::increment_deraumere()
/// @brief Ajoute une unité de deraumere à l'inventaire de l'IA.
void IA::increment_deraumere()
{
    nb_deraumere++;
}

/// @fn void IA::increment_sibur()
/// @brief Ajoute une unité de sibur à l'inventaire de l'IA.
void IA::increment_sibur()
{ 
    nb_sibur++; 
}

/// @fn void IA::increment_mendiane()
/// @brief Ajoute une unité de mendiane à l'inventaire de l'IA.
void IA::increment_mendiane()
{
    nb_mendiane++;
}

/// @fn void IA::increment_phiras()
/// @brief Ajoute une unité de phiras à l'inventaire de l'IA.
void IA::increment_phiras()
{
    nb_phiras++;
}

/// @fn void IA::increment_thystame()
/// @brief Ajoute une unité de thystame à l'inventaire de l'IA.
void IA::increment_thystame()
{ 
    nb_thystame++;
}

/// @fn void IA::increment_level()
/// @brief Augmente le niveau de l'IA de 1.
void IA::increment_level()
{
    level++;
}

/// @fn void IA::decrement_food()
/// @brief Retire une unité de nourriture si l'IA en possède au moins une.
void IA::decrement_food()
{ 
    if (nb_food > 0) 
        nb_food--; 
}