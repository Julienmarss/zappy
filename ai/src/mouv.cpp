/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** mouv
*/

#include "mouv.hpp"

/**
 * @brief Détermine la meilleure direction à prendre selon l’état de l’IA et les objets visibles.
 *
 * Cette fonction parcourt les cases visibles autour du joueur pour choisir la direction optimale
 * en fonction de son état (FOOD_MOD, LINEMATE_MOD, etc.). Elle remplit ensuite les étapes de déplacement
 * et déclenche les actions nécessaires (mouvement, prise d'objet, montée de niveau).
 *
 * @param socket Descripteur du socket pour envoyer les commandes au serveur.
 * @param ia Référence vers l’objet IA contenant l’état, les étapes à suivre et les ressources.
 * @param double_tableau Tableau à deux dimensions contenant les objets visibles autour du joueur.
 */
void choose_mov(int socket, IA& ia, const std::vector<std::vector<std::string>>& double_tableau)
{
    int target_index = -1;
    int max_food_count = 0;
    int max_linemate_count = 0;
    int max_deraumere_count = 0;
    int max_sibur_count = 0;
    int max_mendiane_count = 0;
    int max_phiras_count = 0;
    int max_thystame_count = 0;

    for (size_t index = 0; index < double_tableau.size(); ++index) {
        const auto& tab = double_tableau[index];
        std::cout << "index: " << index << " tab: ";
        
        for (const auto& item : tab) {
            std::cout << item << " ";
        }
        std::cout << "\nmod : " << ia.state << std::endl;

        if (ia.level == 1 && std::find(tab.begin(), tab.end(), "linemate") != tab.end()) {
            target_index = index;
            break;
        } else if (std::find(tab.begin(), tab.end(), "food") != tab.end() && ia.state == "FOOD_MOD") {
            int food_count = std::count(tab.begin(), tab.end(), "food");

            if (food_count > max_food_count) {
                max_food_count = food_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "linemate") != tab.end() && ia.state == "LINEMATE_MOD") {
            int linemate_count = std::count(tab.begin(), tab.end(), "linemate");

            if (linemate_count > max_linemate_count) {
                max_linemate_count = linemate_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "deraumere") != tab.end() && ia.state == "DERAUMERE_MOD") {
            int deraumere_count = std::count(tab.begin(), tab.end(), "deraumere");

            if (deraumere_count > max_deraumere_count) {
                max_deraumere_count = deraumere_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "sibur") != tab.end() && ia.state == "SIBUR_MOD") {
            int sibur_count = std::count(tab.begin(), tab.end(), "sibur");

            if (sibur_count > max_sibur_count) {
                max_sibur_count = sibur_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "mendiane") != tab.end() && ia.state == "MENDIANE_MOD") {
            int mendiane_count = std::count(tab.begin(), tab.end(), "mendiane");

            if (mendiane_count > max_mendiane_count) {
                max_mendiane_count = mendiane_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "phiras") != tab.end() && ia.state == "PHIRAS_MOD") {
            int phiras_count = std::count(tab.begin(), tab.end(), "phiras");

            if (phiras_count > max_phiras_count) {
                max_phiras_count = phiras_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "thystame") != tab.end() && ia.state == "THYSTAME_MOD") {
            int thystame_count = std::count(tab.begin(), tab.end(), "thystame");

            if (thystame_count > max_thystame_count) {
                max_thystame_count = thystame_count;
                target_index = index;
            }
        }
    }

    if (target_index != -1) {
        auto steps = get_steps_to_index(target_index);
        for (const auto& step : steps) {
            ia.steps.push_back(step);
        }
        handle_movement(socket, ia);
        take_action(ia, double_tableau[target_index]);
        level_up(ia);
    } else {
        ia.steps.push_back("Forward");
        ia.steps.push_back("Forward");
        handle_movement(socket, ia);
    }
}

/**
 * @brief Génère les étapes de déplacement nécessaires pour atteindre une case spécifique.
 *
 * Cette fonction retourne une séquence d’instructions ("Forward", "Left", "Right") permettant
 * de se déplacer vers la case spécifiée par son index dans la vue du joueur (indices de 0 à 8).
 *
 * @param index Index de la case cible dans la vision du joueur.
 * @return Un vecteur de chaînes représentant les actions à effectuer.
 */
std::vector<std::string> get_steps_to_index(int index)
{
    switch (index) {
        case 1:
            return {"Forward", "Left", "Forward"};
        case 2:
            return {"Forward"};
        case 3:
            return {"Forward", "Right", "Forward"};
        case 4:
            return {"Forward", "Forward", "Left", "Forward", "Forward"};
        case 5:
            return {"Forward", "Forward", "Left", "Forward"};
        case 6:
            return {"Forward", "Forward"};
        case 7:
            return {"Forward", "Forward", "Right", "Forward"};
        case 8:
            return {"Forward", "Forward", "Right", "Forward", "Forward"};
        default:
            return {};
    }
}


