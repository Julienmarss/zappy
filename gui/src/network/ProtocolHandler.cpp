/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** Protocol Handler
*/

#include "../../include/ProtocolHandler.hpp"
#include "../../include/GameState.hpp"
#include "../../include/Application.hpp"
#include <unordered_map>

namespace Zappy {

ProtocolHandler::ProtocolHandler(GameState* gameState, Application* app) 
    : gameState(gameState), application(app) {}

std::vector<std::string> ProtocolHandler::splitMessage(const std::string& message)
{
    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void ProtocolHandler::handleMapSize(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 3) {
        int width = std::stoi(tokens[1]);
        int height = std::stoi(tokens[2]);
        gameState->setMapSize(width, height);
    }
}

void ProtocolHandler::handleTileContent(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 9) {
        int x = std::stoi(tokens[1]);
        int y = std::stoi(tokens[2]);
        
        std::array<int, 7> resources{};
        for (size_t i = 0; i < 7; ++i) {
            resources[i] = std::stoi(tokens[3 + i]);
        }
        
        gameState->setTileContent(x, y, resources);
    }
}

void ProtocolHandler::handleTeamName(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        gameState->addTeam(tokens[1]);
    }
}

void ProtocolHandler::handlePlayerNew(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 6) {
        int id = std::stoi(tokens[1].substr(1));
        int x = std::stoi(tokens[2]);
        int y = std::stoi(tokens[3]);
        auto orientation = static_cast<Orientation>(std::stoi(tokens[4]));
        int level = std::stoi(tokens[5]);
        const std::string& team = tokens[6];
        
        gameState->addPlayer(id, x, y, orientation, level, team);
    }
}

void ProtocolHandler::handlePlayerPosition(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 5) {
        int id = std::stoi(tokens[1].substr(1));
        int x = std::stoi(tokens[2]);
        int y = std::stoi(tokens[3]);
        auto orientation = static_cast<Orientation>(std::stoi(tokens[4]));
        
        gameState->updatePlayerPosition(id, x, y, orientation);
    }
}

void ProtocolHandler::handlePlayerLevel(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 3) {
        int id = std::stoi(tokens[1].substr(1));
        int newLevel = std::stoi(tokens[2]);
        
        const PlayerData* player = gameState->getPlayer(id);
        int oldLevel = player ? player->level : 1;
        
        gameState->updatePlayerLevel(id, newLevel);
        
        if (newLevel > oldLevel) {
            g_levelUpTriggered = true;
        }
    }
}

void ProtocolHandler::handlePlayerInventory(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 10) {
        int id = std::stoi(tokens[1].substr(1));
        
        std::array<int, 7> inventory{};
        for (size_t i = 0; i < 7; ++i) {
            inventory[i] = std::stoi(tokens[4 + i]);
        }
        
        gameState->updatePlayerInventory(id, inventory);
    }
}

void ProtocolHandler::handlePlayerDeath(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        int id = std::stoi(tokens[1].substr(1));
        gameState->removePlayer(id);
    }
}
        
void ProtocolHandler::handleEggLaid(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 5) {
        int eggId = std::stoi(tokens[1].substr(1));
        int x = std::stoi(tokens[3]);
        int y = std::stoi(tokens[4]);
        
        gameState->addEgg(eggId, x, y);
    }
}

void ProtocolHandler::handleEggDeath(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        int eggId = std::stoi(tokens[1].substr(1));
        gameState->removeEgg(eggId);
    }
}

void ProtocolHandler::handleTimeUnit(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        int timeUnit = std::stoi(tokens[1]);
        gameState->setTimeUnit(timeUnit);
    }
}

void ProtocolHandler::handleEndGame(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        gameState->setGameEnded(tokens[1]);
        g_victoryTriggered = true;
        g_showVictoryPopup = true;
        g_winnerTeamName = tokens[1];
    }
}

void ProtocolHandler::processMessage(const std::string& message)
{
    if (message.empty()) return;
    
    auto tokens = splitMessage(message);
    if (tokens.empty()) return;
    
    const std::string& command = tokens[0];
    
    static const std::unordered_map<std::string, void(ProtocolHandler::*)(const std::vector<std::string>&)> commandMap = {
        {"msz", &ProtocolHandler::handleMapSize},
        {"bct", &ProtocolHandler::handleTileContent},
        {"tna", &ProtocolHandler::handleTeamName},
        {"pnw", &ProtocolHandler::handlePlayerNew},
        {"ppo", &ProtocolHandler::handlePlayerPosition},
        {"plv", &ProtocolHandler::handlePlayerLevel},
        {"pin", &ProtocolHandler::handlePlayerInventory},
        {"pdi", &ProtocolHandler::handlePlayerDeath},
        {"enw", &ProtocolHandler::handleEggLaid},
        {"edi", &ProtocolHandler::handleEggDeath},
        {"sgt", &ProtocolHandler::handleTimeUnit},
        {"seg", &ProtocolHandler::handleEndGame}
    };
    
    auto it = commandMap.find(command);
    if (it != commandMap.end()) {
        (this->*(it->second))(tokens);
    }
}

}