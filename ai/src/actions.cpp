/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** actions
*/

#include "actions.hpp"

/**
 * @brief Analyse la réponse du serveur à la commande Look.
 * 
 * Cette fonction extrait les objets visibles sur chaque case et les organise
 * sous forme de tableau 2D.
 *
 * @param ia Référence vers l’instance de l’IA contenant les réponses à analyser.
 * @return std::vector<std::vector<std::string>> Tableau 2D des objets visibles.
 */
std::vector<std::vector<std::string>> look_and_parse(IA& ia)
{
    std::string decoded_response = ia.reponse.front();
    ia.reponse.pop_front();
    std::cout << "Raw Response: " << decoded_response << std::endl;
    
    std::string content = decoded_response.substr(1, decoded_response.length() - 2);
    std::istringstream iss(content);
    std::string case_str;
    std::vector<std::vector<std::string>> double_tableau;
    
    while (std::getline(iss, case_str, ',')) {
        case_str.erase(0, case_str.find_first_not_of(" \t"));
        case_str.erase(case_str.find_last_not_of(" \t") + 1);
        
        std::istringstream case_iss(case_str);
        std::string object;
        std::vector<std::string> objects;
        
        while (case_iss >> object) {
            objects.push_back(object);
        }
        
        if (objects.empty()) {
            objects.push_back("yarienici");
        }
        
        double_tableau.push_back(objects);
    }
    
    std::cout << "Double Charts:" << std::endl;
    for (const auto& case_vec : double_tableau) {
        for (const auto& obj : case_vec) {
            std::cout << obj << " ";
        }
        std::cout << std::endl;
    }
    
    return double_tableau;
}

/**
 * @brief Génère des commandes Take pour chaque objet détecté utile.
 * 
 * Ignore les objets "yarienici" et "player", et ajoute une commande Take
 * dans les étapes de l’IA pour tous les autres objets.
 * 
 * @param ia Référence vers l’IA dans laquelle on ajoute les commandes.
 * @param items Liste des objets présents sur une case.
 */
void take_action(IA& ia, const std::vector<std::string>& items)
{
    for (const auto& item : items) {
        if (item != "yarienici" && item != "player") {
            std::string command = "Take " + item;
            ia.steps.push_back(command);
        }
    }
}
