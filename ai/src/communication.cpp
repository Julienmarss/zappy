/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** communication
*/

/**
 * @file communication.cpp
 * @brief Gère la logique de communication entre IA pour les rendez-vous et les déplacements synchronisés.
 *
 * Ce fichier contient l’implémentation de la fonction `meetup` qui interprète les messages de broadcast,
 * vérifie l’identifiant de message, et génère les actions appropriées pour que l’IA rejoigne l’équipe.
 */

#include "communication.hpp"
#include "IA.hpp"
#include "mouv.hpp" 
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "inventory.hpp"
#include "take_object.hpp" 

/**
 * @brief Gère la synchronisation et le déplacement de l’IA vers le point de rendez-vous avec l’équipe.
 *
 * Cette fonction analyse le message de broadcast reçu, vérifie s’il correspond à l’identifiant de l’IA,
 * puis génère les mouvements nécessaires pour se rapprocher du point de rendez-vous.
 *
 * @param socket Le descripteur de socket pour envoyer les commandes.
 * @param ia Référence vers l’instance de l’IA contenant les données de l’état actuel.
 */
void meetup(int socket, IA& ia)
{
    std::vector<std::string> tab = split(ia.reponse.front(), ',');
    std::vector<std::string> tab2 = split(tab[0], ' ');
    std::string dir = tab2[1];
    std::vector<std::string> tab3 = split(tab[1], '_');
    int nb = std::stoi(tab3[3]);
    
    if (nb != ia.random_num) {
        return;
    }
    
    if (dir.find("0") != std::string::npos) {
        ia.steps.clear();
        if (!ia.joined) {
            std::string meetup_msg = "Broadcast -" + ia.teamname + "-yadelaplace?-";
            ia.steps.push_back(meetup_msg);
            ia.joined = true;
            ia.nb_player++;
        }
        std::cout << "\n\n\n piper perri \n\n\n" << std::endl;
        handle_movement(socket, ia);
        return;
    } else if (dir.find("3") != std::string::npos) {
        ia.steps.push_back("Left");
    } else if (dir.find("4") != std::string::npos) {
        ia.steps.push_back("Left");
    } else if (dir.find("5") != std::string::npos) {
        ia.steps.push_back("Left");
        ia.steps.push_back("Left");
    } else if (dir.find("6") != std::string::npos) {
        ia.steps.push_back("Right");
    } else if (dir.find("7") != std::string::npos) {
        ia.steps.push_back("Right");
    }
    
    ia.steps.push_back("Forward");
    std::string gooo = "Broadcast _" + ia.teamname + "_jarrivemonquoicoubebousucreausucre_" + std::to_string(ia.random_num);
    ia.steps.push_back(gooo);
    handle_movement(socket, ia);
}