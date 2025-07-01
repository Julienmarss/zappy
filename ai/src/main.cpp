/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** main
*/

#include "IA.hpp"
#include "game.hpp"
#include "login.hpp"

/**
 * @brief Point d’entrée principal de l’IA Zappy.
 *
 * Cette fonction analyse les arguments de la ligne de commande, tente de se connecter au serveur Zappy
 * via la fonction login(), puis lance la boucle de jeu via main_game() si la connexion est réussie.
 *
 * @param argc Nombre d’arguments de la ligne de commande.
 * @param argv Tableau contenant les arguments de la ligne de commande.
 * @return int 0 si l'exécution s'est bien déroulée, 1 en cas d'erreur.
 */
int main(int argc, char* argv[])
{

    IA my_ia;
    int socket_fd;
    std::string host;
    int port = -1;
    std::string name;
    bool has_h = false;

    if (argc < 7) {
        std::cout << "Usage: ./zappy_ai -p <port> -n <name> -h <host>" << std::endl;
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h") {
            has_h = true;
            if (i + 1 < argc) {
                host = argv[i + 1];
                i++;
            }
        } else if (std::string(argv[i]) == "-p") {
            if (i + 1 < argc) {
                port = std::stoi(argv[i + 1]);
                i++;
            }
        } else if (std::string(argv[i]) == "-n") {
            if (i + 1 < argc) {
                name = argv[i + 1];
                i++;
            }
        }
    }
    if (!has_h || port == -1 || name.empty() || host.empty()) {
        std::cout << "Missing required arguments: -p <port>, -n <name>, -h <host>" << std::endl;
        return 1;
    }
    try {
        socket_fd = login(host, port, name, 5, my_ia);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    if (socket_fd > 0) {
        try {
            main_game(socket_fd, my_ia);
        } catch (const std::exception& e) {
            std::cout << "Error in game: " << e.what() << std::endl;
            close(socket_fd);
            return 1;
        }
    } else {
        return 1;
    }
    close(socket_fd);
    return 0;
}