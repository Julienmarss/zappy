/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** take_object
*/

#include "take_object.hpp" 

/**
 * @brief Envoie les commandes de déplacement de l’IA au serveur.
 *
 * Cette fonction envoie jusqu’à 10 commandes de déplacement (ex: "Forward", "Left") à partir de la pile `steps`
 * de l'IA, en les retirant au fur et à mesure. Elle met à jour le compteur de commandes envoyées.
 *
 * @param socket Le descripteur du socket utilisé pour envoyer les données.
 * @param ia Référence vers l’objet IA contenant les commandes à envoyer et le compteur.
 */
void handle_movement(int socket, IA& ia)
{
    while (ia.nb_command_send < 10 && !ia.steps.empty()) {
        std::string command = ia.steps[0] + "\n";
        send(socket, command.c_str(), command.length(), 0);
        ia.nb_command_send++;
        std::cout << "Command execute : " << command;
        ia.steps.erase(ia.steps.begin());
    }
}

/**
 * @brief Analyse les objets visibles et génère les commandes de collecte appropriées.
 *
 * Cette fonction vérifie les objets présents sur la case (ex: "food", "linemate") et, en fonction
 * de l’état de l’IA, décide quelles commandes "Take X" doivent être ajoutées à sa pile d’actions.
 * Elle appelle `handle_movement` après chaque ajout.
 *
 * @param socket Descripteur du socket permettant d’envoyer les commandes.
 * @param ia Référence vers l’IA contenant les informations d’état et les ressources globales.
 * @param tab Vecteur contenant les objets visibles sur la case actuelle.
 */
void take_action(int socket, IA& ia, const std::vector<std::string>& tab)
{
    if (ia.meetup == true && std::find(tab.begin(), tab.end(), "player") != tab.end()) {
        return;
    }
    for (const auto& element : tab) {
        if (element == "food") {
            take_food(ia);
        }
        if ((element == "linemate" && ia.state == "FOOD_MOD") || 
            (element == "linemate" && ia.state == "LINEMATE_MOD" && ia.nb_global_linemate < 8)) {
            take_linemate(ia);
        }
        if ((element == "deraumere" && ia.state == "FOOD_MOD") || 
            (element == "deraumere" && ia.state == "DERAUMERE_MOD" && ia.nb_global_deraumere < 8)) {
            take_deraumere(ia);
        }
        if ((element == "sibur" && ia.state == "FOOD_MOD") || 
            (element == "sibur" && ia.state == "SIBUR_MOD" && ia.nb_global_sibur < 10)) {
            take_sibur(ia);
        }
        if ((element == "mendiane" && ia.state == "FOOD_MOD") || 
            (element == "mendiane" && ia.state == "MENDIANE_MOD" && ia.nb_global_mendiane < 5)) {
            take_mendiane(ia);
        }
        if ((element == "phiras" && ia.state == "FOOD_MOD") || 
            (element == "phiras" && ia.state == "PHIRAS_MOD" && ia.nb_global_phiras < 6)) {
            take_phiras(ia);
        }
        if ((element == "thystame" && ia.state == "FOOD_MOD") || 
            (element == "thystame" && ia.state == "THYSTAME_MOD" && ia.nb_global_thystame < 1)) {
            take_thystame(ia);
        }
        handle_movement(socket, ia);
    }
}


/**
 * @brief Planifie la commande de collecte de nourriture.
 *
 * Ajoute la commande "Take food" à la liste des étapes à effectuer par l’IA.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_food(IA& ia)
{
    ia.steps.push_back("Take food");
}

/**
 * @brief Planifie la commande de collecte de linemate.
 *
 * Ajoute la commande "Take linemate" à la liste des étapes à effectuer par l’IA et incrémente le compteur global.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_linemate(IA& ia)
{
    ia.steps.push_back("Take linemate");
    ia.nb_global_linemate++;
}

/**
 * @brief Planifie la commande de collecte de deraumere.
 *
 * Ajoute la commande "Take deraumere" à la liste des étapes à effectuer par l’IA et incrémente le compteur global.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_deraumere(IA& ia)
{
    ia.steps.push_back("Take deraumere");
    ia.nb_global_deraumere++;
}

/**
 * @brief Planifie la commande de collecte de sibur.
 *
 * Ajoute la commande "Take sibur" à la liste des étapes à effectuer par l’IA et incrémente le compteur global.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_sibur(IA& ia)
{
    ia.steps.push_back("Take sibur");
    ia.nb_global_sibur++;
}

/**
 * @brief Planifie la commande de collecte de mendiane.
 *
 * Ajoute la commande "Take mendiane" à la liste des étapes à effectuer par l’IA et incrémente le compteur global.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_mendiane(IA& ia)
{
    ia.steps.push_back("Take mendiane");
    ia.nb_global_mendiane++;
}

/**
 * @brief Planifie la commande de collecte de phiras.
 *
 * Ajoute la commande "Take phiras" à la liste des étapes à effectuer par l’IA et incrémente le compteur global.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_phiras(IA& ia)
{
    ia.steps.push_back("Take phiras");
    ia.nb_global_phiras++;
}

/**
 * @brief Planifie la commande de collecte de thystame.
 *
 * Ajoute la commande "Take thystame" à la liste des étapes à effectuer par l’IA et incrémente le compteur global.
 *
 * @param ia Référence vers l’objet IA.
 */
void take_thystame(IA& ia) 
{
    ia.steps.push_back("Take thystame");
    ia.nb_global_thystame++;
}
