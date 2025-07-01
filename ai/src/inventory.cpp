/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** inventory
*/

#include "inventory.hpp"
#include "IA.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

/**
 * @brief Met à jour l'inventaire de l'IA à partir de la réponse du serveur.
 * 
 * Cette fonction extrait les ressources de la réponse du serveur et les affecte
 * aux attributs correspondants dans l'objet IA.
 * 
 * @param ia Référence vers l'objet IA contenant les informations d'inventaire.
 */
void get_and_update_inventory(IA& ia)
{
    std::string inventory_after = ia.reponse.front();
    ia.reponse.pop_front();
    std::cout << "Inventory update: " << inventory_after << std::endl;
    std::string content = inventory_after.substr(1, inventory_after.length() - 2);
    std::istringstream iss(content);
    std::string item;
    std::map<std::string, int> inventory_dict;
    while (std::getline(iss, item, ',')) {
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        std::istringstream item_iss(item);
        std::string name, count_str;
        item_iss >> name >> count_str;
        int count = 1;
        if (!count_str.empty() && std::all_of(count_str.begin(), count_str.end(), ::isdigit)) {
            count = std::stoi(count_str);
        }
        
        inventory_dict[name] = count;
    }
    ia.nb_food = inventory_dict["food"];
    ia.nb_linemate = inventory_dict["linemate"];
    ia.nb_deraumere = inventory_dict["deraumere"];
    ia.nb_sibur = inventory_dict["sibur"];
    ia.nb_mendiane = inventory_dict["mendiane"];
    ia.nb_phiras = inventory_dict["phiras"];
    ia.nb_thystame = inventory_dict["thystame"];
}

/**
 * @brief Découpe une chaîne de caractères en sous-chaînes selon un délimiteur.
 * 
 * @param str La chaîne à découper.
 * @param delimiter Le caractère utilisé pour la séparation.
 * @return std::vector<std::string> Liste des sous-chaînes extraites.
 */
std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Met à jour l'inventaire global partagé de l'équipe à partir d'un message broadcast.
 * 
 * Cette fonction analyse un message reçu et met à jour les ressources globales partagées,
 * ou le nombre de joueurs connectés/présents en fonction de l'état et du type du message.
 * 
 * @param ia Référence vers l'objet IA à mettre à jour.
 */
void increment_global_inventory(IA& ia)
{
    std::vector<std::string> rep = split(ia.reponse.front(), ',');
    std::vector<std::string> message = split(rep[1], '-');
    
    if (message[2] == "chief" && message[1] == ia.teamname) {
        ia.not_chief = true;
        ia.chief_is_designed = true;
        ia.reponse.pop_front();
        return;
    }
    
    if (message[2] == "yadelaplace?" && ia.state == "MEET_UP_CHIEF" && ia.teamname == message[1]) {
        ia.nb_player++;
        ia.reponse.pop_front();
        return;
    } else if (message[2] == "yadelaplace?" && ia.teamname == message[1] && ia.state != "MEET_UP_CHIEF") {
        ia.nb_player++;
        ia.reponse.pop_front();
        return;
    }
    
    if (message[2] == "jvaisprendremadouche" && message[1] == ia.teamname && ia.state == "MEET_UP_CHIEF") {
        ia.player_ready++;
        ia.reponse.pop_front();
        return;
    }
    
    if (message[2] == "jvaisprendremadouche" && message[1] == ia.teamname && ia.state == "MEETUP") {
        ia.player_ready++;
        ia.reponse.pop_front();
        return;
    }
    if (message[1] == ia.teamname && message.size() >= 8) {
        ia.nb_global_linemate += std::stoi(message[2]);
        ia.nb_global_deraumere += std::stoi(message[3]);
        ia.nb_global_sibur += std::stoi(message[4]);
        ia.nb_global_mendiane += std::stoi(message[5]);
        ia.nb_global_phiras += std::stoi(message[6]);
        ia.nb_global_thystame += std::stoi(message[7]);
    }
    ia.reponse.pop_front();
}
