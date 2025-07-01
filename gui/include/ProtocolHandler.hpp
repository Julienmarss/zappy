/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** Protocol header
*/

#pragma once
#include "Common.hpp"

namespace Zappy {
    class GameState;
    class Application;

    class ProtocolHandler {
    private:
        GameState* gameState;

        std::vector<std::string> splitMessage(const std::string& message);
        
        void handleMapSize(const std::vector<std::string>& tokens);
        void handleTileContent(const std::vector<std::string>& tokens);
        void handleTeamName(const std::vector<std::string>& tokens);
        void handlePlayerNew(const std::vector<std::string>& tokens);
        void handlePlayerPosition(const std::vector<std::string>& tokens);
        void handlePlayerLevel(const std::vector<std::string>& tokens);
        void handlePlayerInventory(const std::vector<std::string>& tokens);
        void handlePlayerDeath(const std::vector<std::string>& tokens);
        void handleEggLaid(const std::vector<std::string>& tokens);
        void handleEggDeath(const std::vector<std::string>& tokens);
        void handleTimeUnit(const std::vector<std::string>& tokens);
        void handleEndGame(const std::vector<std::string>& tokens);
        Application* application; 

    public:
        explicit ProtocolHandler(GameState* gameState);
        ~ProtocolHandler() = default;

        void processMessage(const std::string& message);
        explicit ProtocolHandler(GameState* gameState, Application* app = nullptr);

    };
}