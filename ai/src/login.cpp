/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** login
*/

#include "login.hpp"

/**
 * @brief Connecte le client IA au serveur Zappy.
 *
 * Cette fonction initialise un socket, configure un timeout,
 * et tente de se connecter au serveur via l'adresse IP et le port donnés.
 * Après la connexion, elle envoie le nom de l’équipe, lit la réponse,
 * et initialise les champs de l’objet IA avec les données reçues.
 *
 * @param ip L’adresse IP du serveur.
 * @param port Le port de connexion.
 * @param name Le nom de l’équipe à envoyer au serveur.
 * @param timeout Temps limite en secondes pour la réception.
 * @param ia Référence vers l’objet IA à initialiser avec les infos de connexion.
 * @return Le descripteur de socket (int) si la connexion réussit.
 * @throws std::runtime_error En cas d’erreur de socket, d’adresse invalide, ou d’échec de connexion.
 */
int login(const std::string& ip, int port, const std::string& name, int timeout, IA& ia)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Error While Creating Socket");
    }

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);
        throw std::runtime_error("Invalide Map");
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        throw std::runtime_error("Error While Connection To Server");
    }

    char buffer[1024];
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Response : " << buffer << std::endl;
    }
    ia.teamname = name;
    std::string message = name + "\n";
    send(sock, message.c_str(), message.length(), 0);
    bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::string response(buffer);
        std::istringstream iss(response);
        std::string line;
        if (std::getline(iss, line)) {
            ia.client_num = line;
        }
        std::cout << "CLIENT: " << ia.client_num << std::endl;
        std::cout << "CLIENT ID: " << response << std::endl;
    }
    return sock;
}