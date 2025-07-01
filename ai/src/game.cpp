/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** game
*/

#include "game.hpp"
#include "IA.hpp"
#include "mouv.hpp"  
#include "communication.hpp" 
#include "inventory.hpp" 
#include "level_handling.hpp" 
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> 
#include "take_object.hpp"  


/**
 * @brief Boucle principale de l’IA.
 *
 * Cette fonction gère le cœur de la logique d’une IA Zappy. Elle lit les messages reçus
 * du serveur, met à jour les états internes de l'IA, traite les commandes, déclenche des mouvements
 * et pilote la montée de niveau.
 *
 * @param socket Descripteur de socket connecté au serveur Zappy.
 * @param ia Référence vers l’instance IA, contenant l’état, les ressources et les étapes à exécuter.
 */
void main_game(int socket, IA& ia)
{
    bool is_look = false;
    bool is_inventory = false;
    bool passeed = false;
    bool fork = true;
    
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket, &readfds);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
        int ready = select(socket + 1, &readfds, nullptr, nullptr, &timeout);
        if (ready > 0 && FD_ISSET(socket, &readfds)) {
            char buffer[1024];
            int bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::string data(buffer);
                std::cout << "Message received: " << data << std::endl;
                
                std::vector<std::string> dbl_tab = split(data, '\n');
                for (const auto& elem : dbl_tab) {
                    if (!elem.empty()) {
                        ia.reponse.push_back(elem);
                    }
                }
                while (!ia.reponse.empty()) {
                    std::string response = ia.reponse.front();
                    if (response.find("food") != std::string::npos && response.find("food") < 6) {
                        ia.nb_command_send--;
                        get_and_update_inventory(ia);
                        passeed = false;
                    } else if (response.find("player") != std::string::npos && response.find("player") < 8 && ia.state != "MEET_UP_CHIEF") {
                        ia.nb_command_send--;
                        auto double_tableau = look_and_parse(ia);
                        choose_mov(socket, ia, double_tableau);
                        passeed = true;
                    } else if (response.find("message") != std::string::npos && response.find("message") < 9) {
                        std::vector<std::string> rep = split(response, ',');
                        if (rep.size() > 1 && rep[1].find("vientonsedate") != std::string::npos && ia.state != "MEET_UP_CHIEF") {
                            std::vector<std::string> current_team = split(rep[1], '_');
                            if (current_team.size() > 1 && current_team[1] == ia.teamname) {
                                std::string gooo = "Broadcast _" + ia.teamname + "_jarrivemonquoicoubebousucreausucre_" + std::to_string(ia.random_num);
                                ia.steps.push_back(gooo);
                                ia.reponse.pop_front();
                                handle_movement(socket, ia);
                            } else {
                                ia.meetup = true;
                                ia.reponse.pop_front();
                            }
                        } else if (rep.size() > 1 && rep[1].find("vientonsedate") != std::string::npos && ia.state == "MEET_UP_CHIEF") {
                            ia.reponse.pop_front();
                        }
                        if (rep.size() > 1 && rep[1].find("_jarrivemonquoicoubebousucreausucre_") != std::string::npos && ia.state == "MEET_UP_CHIEF") {
                            if (ia.nb_player >= 6) {
                                ia.reponse.pop_front();
                                return;
                            }
                            std::vector<std::string> tab3 = split(rep[1], '_');
                            if (tab3.size() > 1 && tab3[1] == ia.teamname) {
                                std::string meetup_perso = "Broadcast _" + ia.teamname + "_meetup_" + tab3[3];
                                ia.steps.push_back(meetup_perso);
                                ia.reponse.pop_front();
                                handle_movement(socket, ia);
                            } else {
                                ia.reponse.pop_front();
                            }
                        } else if (rep.size() > 1 && rep[1].find("_jarrivemonquoicoubebousucreausucre_") != std::string::npos && ia.state != "MEET_UP_CHIEF") {
                            ia.reponse.pop_front();
                        }
                        if (rep.size() > 1 && rep[1].find("_meetup_") != std::string::npos && ia.state != "MEET_UP_CHIEF") {
                            std::vector<std::string> mess = split(rep[1], '_');
                            if (mess.size() > 1 && mess[1] == ia.teamname) {
                                ia.state = "MEETUP";
                                meetup(socket, ia);
                                ia.reponse.pop_front();
                            } else {
                                ia.reponse.pop_front();
                            }
                        } else if (rep.size() > 1 && rep[1].find("_meetup_") != std::string::npos && ia.state == "MEET_UP_CHIEF") {
                            if (ia.nb_player >= 6) {
                                std::cout << "on est tous la" << std::endl;
                                ia.steps.push_back("Inventory");
                                ia.steps.push_back("Incantation");
                                ia.state = "LEVELING_UP";
                                ia.reponse.pop_front();
                                return;
                            }
                            ia.reponse.pop_front();
                        } else if (rep.size() > 1 && rep[1].find("-") != std::string::npos) {
                            increment_global_inventory(ia);
                        }
                    } else if ((response.find("ok") != std::string::npos && response.find("ok") < 4) ||
                               (response.find("ko") != std::string::npos && response.find("ko") < 4)) {
                        ia.nb_command_send--;
                        if (response.find("ko") != std::string::npos && response.find("ko") < 4 && ia.nb_player < 6) {
                            ia.steps.push_back("Left");
                        }
                        ia.reponse.pop_front();
                    } else {
                        ia.reponse.pop_front();
                    }
                }
            } else {
                std::cout << "Server Closed Connexion" << std::endl;
                close(socket);
                exit(1);
            }
        }
        
        if (ia.state != "MEETUP" && ia.state != "MEET_UP_CHIEF") {
            if (ia.nb_food < 20) {
                ia.state = "FOOD_MOD";
            }
            if (ia.nb_food > 35) {
                if (!ia.chief_is_designed) {
                    ia.chief_is_designed = true;
                    ia.is_chief = true;
                    ia.steps.clear();
                    std::string chief = "Broadcast -" + ia.teamname + "-chief";
                    ia.steps.push_back(chief);
                    handle_movement(socket, ia);
                }
                if (!ia.inventory_is_send) {
                    ia.inventory_is_send = true;
                    std::string inv = "Broadcast -" + ia.teamname + "-" + 
                                     std::to_string(ia.nb_linemate) + "-" +
                                     std::to_string(ia.nb_deraumere) + "-" +
                                     std::to_string(ia.nb_sibur) + "-" +
                                     std::to_string(ia.nb_mendiane) + "-" +
                                     std::to_string(ia.nb_phiras) + "-" +
                                     std::to_string(ia.nb_thystame);
                    ia.steps.push_back(inv);
                }
                if (ia.state == "FOOD_MOD") {
                    ia.state = "MANGERRRR";
                }
            }
            
            if (ia.state != "FOOD_MOD" && ia.state != "MEETUP") {
                if (fork) {
                    fork = false;
                    for (int i = 0; i < 5; i++) {
                        ia.steps.push_back("Fork");
                        handle_movement(socket, ia);
                    }
                }

                if (ia.nb_global_linemate < 8) {
                    ia.state = "LINEMATE_MOD";
                } else if (ia.nb_global_deraumere < 8) {
                    ia.state = "DERAUMERE_MOD";
                } else if (ia.nb_global_sibur < 10) {
                    ia.state = "SIBUR_MOD";
                } else if (ia.nb_global_mendiane < 5) {
                    ia.state = "MENDIANE_MOD";
                } else if (ia.nb_global_phiras < 6) {
                    ia.state = "PHIRAS_MOD";
                } else if (ia.nb_global_thystame < 1) {
                    ia.state = "THYSTAME_MOD";
                } else if (ia.state != "MEETUP") {
                    if (ia.is_chief && !ia.not_chief) {
                        ia.state = "MEET_UP_CHIEF";
                        std::string phrase = "Broadcast _" + ia.teamname + "_vientonsedate";
                        ia.steps.push_back(phrase);
                    }
                }
            }
            
            if (!passeed && !is_look && ia.state != "MEETUP" && ia.state != "MEET_UP_CHIEF") {
                ia.steps.push_back("Look");
                is_look = true;
                is_inventory = false;
                handle_movement(socket, ia);
            } else if (passeed && !is_inventory && ia.state != "MEETUP" && ia.state != "MEET_UP_CHIEF") {
                ia.steps.push_back("Inventory");
                handle_movement(socket, ia);
                is_look = false;
                is_inventory = true;
            }
        }
        
        if (ia.nb_player >= 6) {
            go_level_8(ia);
        }
        
        handle_movement(socket, ia);
    }
}