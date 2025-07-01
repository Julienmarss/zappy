/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** IA
*/

#ifndef IA_HPP
#define IA_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <random>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>
#include <sstream>
#include <algorithm>


/**
 * @class IA
 * @brief Représente l'intelligence artificielle d'un joueur dans le projet Zappy.
 *
 * Cette classe stocke toutes les informations nécessaires au fonctionnement de l'IA :
 * inventaire, niveau, état courant, messages reçus, statut du joueur (chef, rejoint...),
 * ainsi que diverses statistiques globales utilisées pour la coordination entre IA.
 */
class IA {
public:
    int nb_food = 0;
    int nb_linemate = 0;
    std::string teamname = "";
    int nb_deraumere = 0;
    int nb_sibur = 0;
    int nb_mendiane = 0;
    int nb_phiras = 0;
    int nb_thystame = 0;
    int nb_global_food = 0;
    int nb_global_linemate = 0;
    int nb_command_send = 0;
    int nb_global_deraumere = 0;
    int nb_global_sibur = 0;
    int nb_global_mendiane = 0;
    int nb_global_phiras = 0;
    std::vector<std::string> steps;
    int nb_global_thystame = 0;
    std::string state = "FOOD_MOD";
    int level = 1;
    std::deque<std::string> reponse;
    int nb_player = 1;
    bool inventory_is_send = false;
    std::string client_num = "-1";
    int random_num;
    bool joined = false;
    bool is_chief = false;
    bool not_chief = false;
    bool chief_is_designed = false;
    bool incantation = false;
    bool meetup = false;
    int player_ready = 1;
    bool broadcast_is_sended = false;

    /**
     * @brief Constructeur de la classe IA.
     */
    IA();

    /**
     * @brief Incrémente le nombre de nourritures détenues.
     */
    void increment_food();

    /**
     * @brief Incrémente le nombre de linemate.
     */
    void increment_linemate();

    /**
     * @brief Incrémente le nombre de déramère.
     */
    void increment_deraumere();

    /**
     * @brief Incrémente le nombre de sibur.
     */
    void increment_sibur();

    /**
     * @brief Incrémente le nombre de mendiane.
     */
    void increment_mendiane();

    /**
     * @brief Incrémente le nombre de phiras.
     */
    void increment_phiras();

    /**
     * @brief Incrémente le nombre de thystame.
     */
    void increment_thystame();

    /**
     * @brief Incrémente le niveau du joueur.
     */
    void increment_level();

    /**
     * @brief Décrémente la nourriture si possible.
     */
    void decrement_food();
};

#endif // IA_HPP