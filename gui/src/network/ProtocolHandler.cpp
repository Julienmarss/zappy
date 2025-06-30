#include "../../include/ProtocolHandler.hpp"
#include "../../include/GameState.hpp"

namespace Zappy {

ProtocolHandler::ProtocolHandler(GameState* gameState) : gameState(gameState) {}

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
        
        #ifdef DEBUG
        std::cout << "Map size: " << width << "x" << height << std::endl;
        #endif
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
        
        #ifdef DEBUG
        std::cout << "Team added: " << tokens[1] << std::endl;
        #endif
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
        
        #ifdef DEBUG
        std::cout << "New player: ID=" << id << " pos=(" << x << "," << y 
                  << ") team=" << team << std::endl;
        #endif
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
        int level = std::stoi(tokens[2]);
        
        gameState->updatePlayerLevel(id, level);
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
        
        #ifdef DEBUG
        std::cout << "Player " << id << " died" << std::endl;
        #endif
    }
}
        
void ProtocolHandler::handleEggLaid(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 5) {
        int eggId = std::stoi(tokens[1].substr(1));
        int x = std::stoi(tokens[3]);
        int y = std::stoi(tokens[4]);
        
        gameState->addEgg(eggId, x, y);
        
        #ifdef DEBUG
        std::cout << "Egg " << eggId << " laid at (" << x << "," << y << ")" << std::endl;
        #endif
    }
}

void ProtocolHandler::handleEggDeath(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        int eggId = std::stoi(tokens[1].substr(1));
        gameState->removeEgg(eggId);
        
        #ifdef DEBUG
        std::cout << "Egg " << eggId << " died" << std::endl;
        #endif
    }
}

void ProtocolHandler::handleTimeUnit(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        int timeUnit = std::stoi(tokens[1]);
        gameState->setTimeUnit(timeUnit);
        
        #ifdef DEBUG
        std::cout << "Time unit: " << timeUnit << std::endl;
        #endif
    }
}

void ProtocolHandler::handleEndGame(const std::vector<std::string>& tokens)
{
    if (tokens.size() >= 2) {
        gameState->setGameEnded(tokens[1]);
        std::cout << "Game ended! Winner: " << tokens[1] << std::endl;
    }
}

void ProtocolHandler::processMessage(const std::string& message)
{
    if (message.empty()) {
        return;
    }
    
    auto tokens = splitMessage(message);
    if (tokens.empty()) {
        return;
    }
    
    const std::string& command = tokens[0];
    
    if (command == "msz") {
        handleMapSize(tokens);
    } else if (command == "bct") {
        handleTileContent(tokens);
    } else if (command == "tna") {
        handleTeamName(tokens);
    } else if (command == "pnw") {
        handlePlayerNew(tokens);
    } else if (command == "ppo") {
        handlePlayerPosition(tokens);
    } else if (command == "plv") {
        handlePlayerLevel(tokens);
    } else if (command == "pin") {
        handlePlayerInventory(tokens);
    } else if (command == "pdi") {
        handlePlayerDeath(tokens);
    } else if (command == "enw") {
        handleEggLaid(tokens);
    } else if (command == "edi") {
        handleEggDeath(tokens);
    } else if (command == "sgt") {
        handleTimeUnit(tokens);
    } else if (command == "seg") {
        handleEndGame(tokens);
    } else if (command == "mct") {
        // Map content - handled by multiple bct commands
    } else if (command == "pex") {
        // Player expulsion - visual effect only
    } else if (command == "pbc") {
        // Player broadcast - could show chat
    } else if (command == "pic") {
        // Incantation start - visual effect
    } else if (command == "pie") {
        // Incantation end - visual effect
    } else if (command == "pfk") {
        // Player fork - handled by enw
    } else if (command == "pdr") {
        // Player drop - handled by tile updates
    } else if (command == "pgt") {
        // Player take - handled by tile updates
    } else if (command == "ebo") {
        // Egg hatching - handled by pnw
    } else if (command == "smg") {
        // Server message
        if (tokens.size() >= 2) {
            std::cout << "Server message: " << tokens[1] << std::endl;
        }
    } else {
        #ifdef DEBUG
        std::cout << "Unknown command: " << command << std::endl;
        #endif
    }
}

}