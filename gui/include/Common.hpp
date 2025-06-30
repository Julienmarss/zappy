#pragma once

#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <queue>
#include <array>
#include <cstring>
#include <algorithm>
#include <sstream> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace Zappy {

    constexpr Color PASTEL_BLUE = {173, 216, 230, 200};       // Bleu pastel avec transparence
    constexpr Color PASTEL_GREEN = {144, 238, 144, 200};      // Vert pastel
    constexpr Color PASTEL_PINK = {255, 182, 193, 200};       // Rose pastel
    constexpr Color PASTEL_PURPLE = {221, 160, 221, 200};     // Violet pastel
    constexpr Color PASTEL_YELLOW = {255, 255, 224, 200};     // Jaune pastel
    constexpr Color PASTEL_ORANGE = {255, 218, 185, 200};     // Orange pastel
    constexpr Color PASTEL_MINT = {152, 251, 152, 200};       // Menthe pastel
    
    // Couleurs pour les bordures
    constexpr Color HUD_BORDER = {100, 100, 120, 255};        // Bordure foncée
    constexpr Color HUD_ACCENT = {200, 200, 220, 255};
    constexpr Color DARK_RED = {139, 0, 0, 255};

    // Test
    constexpr Color NEW = { 124, 69, 153, 255 };

    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr int TILE_SIZE = 32;
    constexpr int MAX_TEAMS = 10;
    constexpr int MAX_PLAYERS = 1000;
    constexpr int MAX_EGGS = 1000;
    constexpr int BUFFER_SIZE = 4096;

    enum class ResourceType {
        FOOD = 0,
        LINEMATE = 1,
        DERAUMERE = 2,
        SIBUR = 3,
        MENDIANE = 4,
        PHIRAS = 5,
        THYSTAME = 6,
        COUNT = 7
    };

    enum class Orientation {
        NORTH = 1,
        EAST = 2,
        SOUTH = 3,
        WEST = 4
    };

    struct Position {
        int x, y;
        Position(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct TileData {
        std::array<int, static_cast<size_t>(ResourceType::COUNT)> resources{};
        std::vector<int> players;
        std::vector<int> eggs;
    };

    struct PlayerData {
        int id{0};
        Position pos{};
        Orientation orientation{Orientation::NORTH};
        int level{1};
        std::string teamName;
        std::array<int, static_cast<size_t>(ResourceType::COUNT)> inventory{};
        bool alive{false};
    };

    struct TeamData {
        std::string name;
        Color color{WHITE};
        int playerCount{0};
    };

    struct EggData {
        int id{0};
        Position pos{};
        std::string teamName;
    };
}