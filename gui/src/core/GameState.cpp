#include "../../include/GameState.hpp"

namespace Zappy {

bool GameState::isValidPosition(const Position& pos) const
{
    return pos.x >= 0 && pos.x < mapSize.x && pos.y >= 0 && pos.y < mapSize.y;
}

void GameState::setMapSize(int width, int height)
{
    mapSize = Position(width, height);
    
    map.clear();
    map.resize(height);
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = TileData{};
        }
    }
    
    #ifdef DEBUG
    std::cout << "GameState: Map initialized with size " << width << "x" << height << std::endl;
    #endif
}

void GameState::setTileContent(int x, int y, const std::array<int, 7>& resources)
{
    Position pos(x, y);
    if (!isValidPosition(pos)) {
        #ifdef DEBUG
        std::cerr << "Invalid tile position: (" << x << "," << y << ")" << std::endl;
        #endif
        return;
    }
    
    map[y][x].resources = resources;
}

const TileData* GameState::getTile(int x, int y) const
{
    Position pos(x, y);
    if (!isValidPosition(pos)) {
        return nullptr;
    }
    
    return &map[y][x];
}

TileData* GameState::getTile(int x, int y)
{
    Position pos(x, y);
    if (!isValidPosition(pos)) {
        return nullptr;
    }
    
    return &map[y][x];
}

void GameState::removePlayerFromTile(int playerId, const Position& pos)
{
    if (!isValidPosition(pos)) return;
    
    auto& tile = map[pos.y][pos.x];
    auto it = std::find(tile.players.begin(), tile.players.end(), playerId);
    if (it != tile.players.end()) {
        tile.players.erase(it);
    }
}

void GameState::addPlayerToTile(int playerId, const Position& pos)
{
    if (!isValidPosition(pos)) return;
    
    auto& tile = map[pos.y][pos.x];
    auto it = std::find(tile.players.begin(), tile.players.end(), playerId);
    if (it == tile.players.end()) {
        tile.players.push_back(playerId);
    }
}

void GameState::addPlayer(int id, int x, int y, Orientation orientation, 
                         int level, const std::string& team)
{
    Position pos(x, y);
    
    PlayerData player;
    player.id = id;
    player.pos = pos;
    player.orientation = orientation;
    player.level = level;
    player.teamName = team;
    player.inventory.fill(0);
    player.alive = true;
    
    auto it = players.find(id);
    if (it != players.end() && it->second.alive) {
        removePlayerFromTile(id, it->second.pos);
    }
    
    players[id] = player;
    addPlayerToTile(id, pos);
    
    #ifdef DEBUG
    std::cout << "GameState: Added player " << id << " at (" << x << "," << y 
              << ") team=" << team << std::endl;
    #endif
}

void GameState::updatePlayerPosition(int id, int x, int y, Orientation orientation)
{
    auto it = players.find(id);
    if (it == players.end() || !it->second.alive) {
        #ifdef DEBUG
        std::cerr << "Player " << id << " not found for position update" << std::endl;
        #endif
        return;
    }
    
    PlayerData& player = it->second;
    
    removePlayerFromTile(id, player.pos);
    
    player.pos = Position(x, y);
    player.orientation = orientation;
    
    addPlayerToTile(id, player.pos);
    
    #ifdef DEBUG
    std::cout << "GameState: Player " << id << " moved to (" << x << "," << y << ")" << std::endl;
    #endif
}

void GameState::updatePlayerLevel(int id, int level)
{
    auto it = players.find(id);
    if (it != players.end() && it->second.alive) {
        it->second.level = level;
        
        #ifdef DEBUG
        std::cout << "GameState: Player " << id << " level updated to " << level << std::endl;
        #endif
    }
}

void GameState::updatePlayerInventory(int id, const std::array<int, 7>& inventory)
{
    auto it = players.find(id);
    if (it != players.end() && it->second.alive) {
        it->second.inventory = inventory;
        
        #ifdef DEBUG
        std::cout << "GameState: Player " << id << " inventory updated" << std::endl;
        #endif
    }
}

void GameState::removePlayer(int id)
{
    auto it = players.find(id);
    if (it == players.end() || !it->second.alive) {
        return;
    }
    
    const PlayerData& player = it->second;
    
    removePlayerFromTile(id, player.pos);
    
    it->second.alive = false;
    
    #ifdef DEBUG
    std::cout << "GameState: Removed player " << id << std::endl;
    #endif
}

const PlayerData* GameState::getPlayer(int id) const
{
    auto it = players.find(id);
    return (it != players.end() && it->second.alive) ? &it->second : nullptr;
}

void GameState::addTeam(const std::string& name)
{
    if (teams.find(name) != teams.end()) {
        return;
    }
    
    TeamData team;
    team.name = name;
    team.playerCount = 0;
    
    static const std::vector<Color> teamColors = {
        RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, LIME, MAROON, DARKBLUE
    };
    
    team.color = teamColors[teams.size() % teamColors.size()];
    teams[name] = team;
    
    #ifdef DEBUG
    std::cout << "GameState: Added team " << name << std::endl;
    #endif
}

const TeamData* GameState::getTeam(const std::string& name) const
{
    auto it = teams.find(name);
    return (it != teams.end()) ? &it->second : nullptr;
}

void GameState::removeEggFromTile(int eggId, const Position& pos)
{
    if (!isValidPosition(pos)) return;
    
    auto& tile = map[pos.y][pos.x];
    auto it = std::find(tile.eggs.begin(), tile.eggs.end(), eggId);
    if (it != tile.eggs.end()) {
        tile.eggs.erase(it);
    }
}

void GameState::addEggToTile(int eggId, const Position& pos)
{
    if (!isValidPosition(pos)) return;
    
    auto& tile = map[pos.y][pos.x];
    auto it = std::find(tile.eggs.begin(), tile.eggs.end(), eggId);
    if (it == tile.eggs.end()) {
        tile.eggs.push_back(eggId);
    }
}

void GameState::addEgg(int id, int x, int y)
{
    Position pos(x, y);
    
    EggData egg;
    egg.id = id;
    egg.pos = pos;
    
    auto it = eggs.find(id);
    if (it != eggs.end()) {
        removeEggFromTile(id, it->second.pos);
    }
    
    eggs[id] = egg;
    addEggToTile(id, pos);
    
    #ifdef DEBUG
    std::cout << "GameState: Added egg " << id << " at (" << x << "," << y << ")" << std::endl;
    #endif
}

void GameState::removeEgg(int id)
{
    auto it = eggs.find(id);
    if (it == eggs.end()) {
        return;
    }
    
    const EggData& egg = it->second;
    
    removeEggFromTile(id, egg.pos);
    
    eggs.erase(it);
    
    #ifdef DEBUG
    std::cout << "GameState: Removed egg " << id << std::endl;
    #endif
}

void GameState::setGameEnded(const std::string& winner)
{
    gameEnded = true;
    winnerTeam = winner;
    
    std::cout << "GameState: Game ended, winner: " << winner << std::endl;
}

}