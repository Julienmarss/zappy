#pragma once
#include "Common.hpp"

namespace Zappy {
    class GameState {
    private:
        Position mapSize{0, 0};
        std::vector<std::vector<TileData>> map;
        std::unordered_map<int, PlayerData> players;
        std::unordered_map<std::string, TeamData> teams;
        std::unordered_map<int, EggData> eggs;
        
        int timeUnit{100};
        bool gameEnded{false};
        std::string winnerTeam;

        void removePlayerFromTile(int playerId, const Position& pos);
        void addPlayerToTile(int playerId, const Position& pos);
        void removeEggFromTile(int eggId, const Position& pos);
        void addEggToTile(int eggId, const Position& pos);
        bool isValidPosition(const Position& pos) const;

    public:
        GameState() = default;
        ~GameState() = default;

        void setMapSize(int width, int height);
        Position getMapSize() const { return mapSize; }
        void setTileContent(int x, int y, const std::array<int, 7>& resources);
        const TileData* getTile(int x, int y) const;
        TileData* getTile(int x, int y);

        void addPlayer(int id, int x, int y, Orientation orientation, 
                      int level, const std::string& team);
        void updatePlayerPosition(int id, int x, int y, Orientation orientation);
        void updatePlayerLevel(int id, int level);
        void updatePlayerInventory(int id, const std::array<int, 7>& inventory);
        void removePlayer(int id);
        const PlayerData* getPlayer(int id) const;
        const std::unordered_map<int, PlayerData>& getPlayers() const { return players; }

        void addTeam(const std::string& name);
        const TeamData* getTeam(const std::string& name) const;
        const std::unordered_map<std::string, TeamData>& getTeams() const { return teams; }
        void addEgg(int id, int x, int y);
        void removeEgg(int id);
        const std::unordered_map<int, EggData>& getEggs() const { return eggs; }
        void setTimeUnit(int unit) { timeUnit = unit; }
        int getTimeUnit() const { return timeUnit; }
        void setGameEnded(const std::string& winner);
        bool isGameEnded() const { return gameEnded; }
        const std::string& getWinnerTeam() const { return winnerTeam; }
    };
}