/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** level_handling
*/

#include "IA.hpp"
#include "level_handling.hpp"
#include "mouv.hpp"
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Prépare les ressources et initie une incantation de passage au niveau 8.
 * 
 * Si l'IA est chef et que suffisamment de joueurs sont présents, déclenche l'incantation.
 * Sinon, envoie un message broadcast pour coordonner le regroupement.
 * 
 * @param ia Référence vers l'IA à mettre à jour.
 */
void go_level_8(IA& ia)
{
    while (ia.nb_linemate > 0) {
        ia.steps.push_back("Set linemate");
        ia.nb_linemate--;
    }
    while (ia.nb_deraumere > 0) {
        ia.steps.push_back("Set deraumere");
        ia.nb_deraumere--;
    }
    while (ia.nb_sibur > 0) {
        ia.steps.push_back("Set sibur");
        ia.nb_sibur--;
    }
    while (ia.nb_mendiane > 0) {
        ia.steps.push_back("Set mendiane");
        ia.nb_mendiane--;
    }
    while (ia.nb_phiras > 0) {
        ia.steps.push_back("Set phiras");
        ia.nb_phiras--;
    }
    while (ia.nb_thystame > 0) {
        ia.steps.push_back("Set thystame");
        ia.nb_thystame--;
    }
    if (ia.state != "MEET_UP_CHIEF" && ia.broadcast_is_sended == false) {
        ia.broadcast_is_sended = true;
        std::string broadcast = "Broadcast -" + ia.teamname + "-jvaisprendremadouche-";
        ia.steps.push_back(broadcast);
    }
    if (ia.state == "MEET_UP_CHIEF" && ia.incantation == false && ia.player_ready >= 6) {
        ia.incantation = true;
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
    }
}

/**
 * @brief Effectue le passage du niveau 1 au niveau 2 si les conditions sont remplies.
 * 
 * Nécessite 1 pierre linemate et être actuellement au niveau 1.
 * 
 * @param ia Référence vers l'IA contenant l’état actuel.
 */
void level_1_to_2(IA& ia)
{
    if (ia.nb_linemate >= 1 && ia.level == 1) {
        std::cout << "\n\ndonne ton insta mon quoicoubebou\n\n" << std::endl;
        ia.steps.push_back("Set linemate");
        ia.steps.push_back("Incantation");
        ia.increment_level();
        ia.nb_linemate--;
        ia.nb_global_linemate--;
    }
}

/**
 * @brief Effectue le passage du niveau 2 au niveau 3 si les conditions sont remplies.
 * 
 * Conditions : 2 joueurs minimum, 1 linemate, 1 deraumere et 1 sibur au sol.
 * 
 * @param ia Référence vers l’IA contenant l’état actuel.
 */
void level_2_to_3(IA& ia)
{
    if (ia.nb_player >= 2 && ia.level == 2 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 1 && ia.nb_global_sibur == 1) {
        std::cout << "\n\nun petit facetime ma babygirl\n\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

/**
 * @brief Effectue le passage du niveau 3 au niveau 4 si les conditions sont remplies.
 * 
 * Conditions : 2 joueurs minimum, 2 linemate, 1 sibur et 2 phiras au sol.
 * 
 * @param ia Référence vers l’IA contenant l’état actuel.
 */
void level_3_to_4(IA& ia)
{
    if (ia.nb_player >= 2 && ia.level == 3 && ia.nb_global_linemate == 2 && 
        ia.nb_global_sibur == 1 && ia.nb_global_phiras == 2) {
        std::cout << "\n\ny'a personne a la maison tu viens combler le vide ;)\n\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

/**
 * @brief Effectue le passage du niveau 4 au niveau 5 si les conditions sont remplies.
 * 
 * Conditions : 4 joueurs minimum, 1 linemate, 1 deraumere, 2 sibur et 1 phiras.
 * 
 * @param ia Référence vers l’IA contenant l’état actuel.
 */
void level_4_to_5(IA& ia)
{
    if (ia.nb_player >= 4 && ia.level == 4 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 1 && ia.nb_global_sibur == 2 && ia.nb_global_phiras == 1) {
        std::cout << "\n\nquoicoubebouuuuuuuu j'ai un retard sur mes regles :(\n\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

/**
 * @brief Effectue le passage du niveau 5 au niveau 6 si les conditions sont remplies.
 * 
 * Conditions : 4 joueurs minimum, 1 linemate, 2 deraumere, 1 sibur et 3 mendiane.
 * 
 * @param ia Référence vers l’IA contenant l’état actuel.
 */
void level_5_to_6(IA& ia)
{
    if (ia.nb_player >= 4 && ia.level == 5 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 2 && ia.nb_global_sibur == 1 && ia.nb_global_mendiane == 3) {
        std::cout << "\n\nmaintenant qu'on a l'enfant on est obligé de se marier\n\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_mendiane > 0) {
            ia.steps.push_back("Set mendiane");
            ia.nb_mendiane--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

/**
 * @brief Effectue le passage du niveau 6 au niveau 7 si les conditions sont remplies.
 * 
 * Conditions : 6 joueurs minimum, 1 linemate, 2 deraumere, 3 sibur et 1 phiras.
 * 
 * @param ia Référence vers l’IA contenant l’état actuel.
 */
void level_6_to_7(IA& ia) {
    if (ia.nb_player >= 6 && ia.level == 6 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 2 && ia.nb_global_sibur == 3 && ia.nb_global_phiras == 1) {
        std::cout << "\n\nc'est qui ce o'tacos qui t'envoie des messages a 3 heures du matin\n\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

/**
 * @brief Effectue le passage du niveau 7 au niveau 8 si les conditions sont remplies.
 * 
 * Conditions : 6 joueurs minimum, 2 linemate, 2 deraumere, 2 sibur, 2 mendiane, 2 phiras et 1 thystame.
 * 
 * @param ia Référence vers l’IA contenant l’état actuel.
 */
void level_7_to_8(IA& ia)
{
    if (ia.nb_player >= 6 && ia.level == 7 && ia.nb_global_linemate == 2 && 
        ia.nb_global_deraumere == 2 && ia.nb_global_sibur == 2 && ia.nb_global_mendiane == 2 && 
        ia.nb_global_phiras == 2 && ia.nb_thystame == 1) {
        std::cout << "\n\nYOU ARE..... NOT THE FATHER\n\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_mendiane > 0) {
            ia.steps.push_back("Set mendiane");
            ia.nb_mendiane--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        while (ia.nb_thystame > 0) {
            ia.steps.push_back("Set thystame");
            ia.nb_thystame--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

/**
 * @brief Tente chaque montée de niveau dans l'ordre, de 1 à 8.
 * 
 * Appelle les fonctions de changement de niveau dans l’ordre croissant.
 * 
 * @param ia Référence vers l’IA à faire progresser.
 */
void level_up(IA& ia)
{
    level_1_to_2(ia);
    level_2_to_3(ia);
    level_3_to_4(ia);
    level_4_to_5(ia);
    level_5_to_6(ia);
    level_6_to_7(ia);
    level_7_to_8(ia);
}
