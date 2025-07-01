/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** Renderer
*/

#include "../../include/Renderer.hpp"
#include "../../include/GameState.hpp"
#include "../../include/Camera.hpp"

namespace Zappy {

Renderer::~Renderer()
{
    if (resourcesLoaded) {
        UnloadFont(font);
    }
}

bool Renderer::loadResources()
{
    font = LoadFont("assets/fonts/arial.ttf");
    if (font.texture.id == 0) {
        font = GetFontDefault();
    }
    
    resourcesLoaded = true;
    return true;
}

const char* Renderer::getResourceName(ResourceType type) const
{
    static const char* names[] = {
        "food", "linemate", "deraumere", "sibur", 
        "mendiane", "phiras", "thystame"
    };
    
    int index = static_cast<int>(type);
    if (index >= 0 && index < static_cast<int>(ResourceType::COUNT)) {
        return names[index];
    }
    return "unknown";
}

Color Renderer::getResourceColor(ResourceType type) const
{
    switch (type) {
        case ResourceType::FOOD:      return GREEN;
        case ResourceType::LINEMATE:  return WHITE;
        case ResourceType::DERAUMERE: return YELLOW;
        case ResourceType::SIBUR:     return BLUE;
        case ResourceType::MENDIANE:  return RED;
        case ResourceType::PHIRAS:    return PURPLE;
        case ResourceType::THYSTAME:  return ORANGE;
        default:                      return GRAY;
    }
}

Color Renderer::getTeamColor(const std::string& teamName) const
{
    if (teamName.empty()) return WHITE;
    
    std::hash<std::string> hasher;
    size_t hash = hasher(teamName);
    
    static const std::vector<Color> colors = {
        RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, LIME
    };
    
    return colors[hash % colors.size()];
}

void Renderer::renderFrame(const GameState& gameState, const Camera& camera)
{
    BeginMode2D(camera.getRaylibCamera());
    
    renderMap(gameState, camera);
    renderPlayers(gameState);
    
    EndMode2D();
}

void Renderer::renderMap(const GameState& gameState, const Camera& camera)
{
    Position mapSize = gameState.getMapSize();
    if (mapSize.x == 0 || mapSize.y == 0) {
        return;
    }
    
    Vector2 topLeft = camera.screenToWorld({0, 0});
Vector2 bottomRight = camera.screenToWorld({(float)GetScreenWidth(), (float)GetScreenHeight()});    
int startX = std::max(0, static_cast<int>(topLeft.x / BASE_TILE_SIZE) - 1);
int endX = std::min(mapSize.x, static_cast<int>(bottomRight.x / BASE_TILE_SIZE) + 2);
int startY = std::max(0, static_cast<int>(topLeft.y / BASE_TILE_SIZE) - 1);
int endY = std::min(mapSize.y, static_cast<int>(bottomRight.y / BASE_TILE_SIZE) + 2);
    
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            const TileData* tile = gameState.getTile(x, y);
            if (tile) {
                renderTile(*tile, x, y);
            }
        }
    }
}

void Renderer::renderTile(const TileData& tile, int x, int y)
{
float tileX = x * BASE_TILE_SIZE;
float tileY = y * BASE_TILE_SIZE;
    
    Color baseColor = {25, 25, 35, 255};
    Color topColor = baseColor;
    Color bottomColor = baseColor;
    
    if (!tile.players.empty()) {
        topColor = {45, 65, 85, 255};
        bottomColor = {35, 45, 65, 255};
    } else if (!tile.eggs.empty()) {
        topColor = {65, 55, 35, 255};
        bottomColor = {45, 35, 25, 255};
    } else {
        int totalResources = 0;
        for (int res : tile.resources) totalResources += res;
        
        if (totalResources > 5) {
            topColor = {35, 45, 25, 255};
            bottomColor = {25, 35, 15, 255};
        }
    }
    
    DrawRectangleGradientV(static_cast<int>(tileX), static_cast<int>(tileY), 
                           BASE_TILE_SIZE, BASE_TILE_SIZE, topColor, bottomColor);
    
    Color borderColor = {80, 80, 90, 255};
    
    DrawRectangleLines(static_cast<int>(tileX), static_cast<int>(tileY), 
                       BASE_TILE_SIZE, BASE_TILE_SIZE, borderColor);
    
    DrawRectangleLines(static_cast<int>(tileX + 1), static_cast<int>(tileY + 1), 
                       BASE_TILE_SIZE - 2, BASE_TILE_SIZE - 2, Fade(borderColor, 0.5f));
    
    Color shadowColor = {0, 0, 0, 30};
    DrawRectangle(static_cast<int>(tileX + BASE_TILE_SIZE - 2), static_cast<int>(tileY + 2), 
                  2, BASE_TILE_SIZE - 2, shadowColor);
    DrawRectangle(static_cast<int>(tileX + 2), static_cast<int>(tileY + BASE_TILE_SIZE - 2), 
                  BASE_TILE_SIZE - 2, 2, shadowColor);
    
    renderTileResources(tile, tileX, tileY);
    
    for (size_t i = 0; i < tile.eggs.size(); ++i) {
        float eggX = tileX + BASE_TILE_SIZE/2;
        float eggY = tileY + BASE_TILE_SIZE/2;
        
        DrawCircle(static_cast<int>(eggX), static_cast<int>(eggY), 8, Fade(YELLOW, 0.3f));
        DrawCircle(static_cast<int>(eggX), static_cast<int>(eggY), 6, WHITE);
        DrawCircleLines(static_cast<int>(eggX), static_cast<int>(eggY), 6, GOLD);
    }
}

void Renderer::renderTileResources(const TileData& tile, float tileX, float tileY)
{
    int resourceCount = 0;
    const int maxResourcesShown = 9;
    
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT) && resourceCount < maxResourcesShown; ++i) {
        for (int j = 0; j < tile.resources[i] && resourceCount < maxResourcesShown; ++j) {
            float offsetX = (resourceCount % 3) * (BASE_TILE_SIZE / 3) + BASE_TILE_SIZE / 6;
            float offsetY = (resourceCount / 3) * (BASE_TILE_SIZE / 3) + BASE_TILE_SIZE / 6;
            
            Color color = getResourceColor(static_cast<ResourceType>(i));
            
            if (i == 0) {
                DrawCircle(static_cast<int>(tileX + offsetX), 
                          static_cast<int>(tileY + offsetY), 4, Fade(color, 0.3f));
                DrawCircle(static_cast<int>(tileX + offsetX), 
                          static_cast<int>(tileY + offsetY), 3, color);
            } else {
                Vector2 center = {tileX + offsetX, tileY + offsetY};
                
                Vector2 points[4] = {
                    {center.x, center.y - 4},
                    {center.x + 3, center.y},
                    {center.x, center.y + 4},
                    {center.x - 3, center.y}
                };
                
                for (int p = 0; p < 4; ++p) {
                    Vector2 next = points[(p + 1) % 4];
                    DrawLineEx({points[p].x + 1, points[p].y + 1}, 
                              {next.x + 1, next.y + 1}, 2, Fade(BLACK, 0.3f));
                }
                
                for (int p = 0; p < 4; ++p) {
                    Vector2 next = points[(p + 1) % 4];
                    DrawLineEx(points[p], next, 2, color);
                }
                
                DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), 1, WHITE);
            }
            
            resourceCount++;
        }
    }
    
    int totalResources = 0;
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        totalResources += tile.resources[i];
    }
    
    if (totalResources > maxResourcesShown) {
        DrawCircle(static_cast<int>(tileX + BASE_TILE_SIZE - 8), 
                  static_cast<int>(tileY + BASE_TILE_SIZE - 8), 6, Fade(WHITE, 0.8f));
        DrawText("+", static_cast<int>(tileX + BASE_TILE_SIZE - 10), 
                static_cast<int>(tileY + BASE_TILE_SIZE - 12), 10, BLACK);
    }
}

void Renderer::renderPlayers(const GameState& gameState)
{
    const auto& players = gameState.getPlayers();
    
    for (const auto& pair : players) {
        const PlayerData& player = pair.second;
        if (!player.alive) continue;
        
        float x = player.pos.x * BASE_TILE_SIZE + BASE_TILE_SIZE / 2;
        float y = player.pos.y * BASE_TILE_SIZE + BASE_TILE_SIZE / 2;
        
        const TeamData* team = gameState.getTeam(player.teamName);
        Color teamColor = team ? team->color : WHITE;

        float radius = BASE_TILE_SIZE / 4 + (player.level - 1) * 1.5f;
        
        DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, teamColor);
        DrawCircleLines(static_cast<int>(x), static_cast<int>(y), radius, BLACK);
        
        Vector2 direction = {0, 0};
        switch (player.orientation) {
            case Orientation::NORTH: direction = {0, -1}; break;
            case Orientation::EAST:  direction = {1, 0}; break;
            case Orientation::SOUTH: direction = {0, 1}; break;
            case Orientation::WEST:  direction = {-1, 0}; break;
        }
        
        Vector2 endPoint = {
            x + direction.x * (radius + 5),
            y + direction.y * (radius + 5)
        };
        
        DrawLineEx({x, y}, endPoint, 3, WHITE);
        
        std::string idText = std::to_string(player.id);
        Vector2 textSize = MeasureTextEx(font, idText.c_str(), 10, 1);
        
        DrawRectangle(static_cast<int>(x - textSize.x/2 - 1), 
                     static_cast<int>(y - radius - 15), 
                     static_cast<int>(textSize.x + 2), 12, BLACK);
        
        DrawTextEx(font, idText.c_str(), 
                   {x - textSize.x/2, y - radius - 12}, 10, 1, WHITE);
        
        for (int level = 1; level < player.level && level <= 5; ++level) {
            float starX = x - 8 + level * 3;
            float starY = y + radius + 5;
            DrawCircle(static_cast<int>(starX), static_cast<int>(starY), 2, GOLD);
        }
        if (player.level > 5) {
            std::string levelText = "L" + std::to_string(player.level);
            DrawTextEx(font, levelText.c_str(), 
                       {x - 8, y + radius + 2}, 8, 1, GOLD);
        }
    }
}

void Renderer::renderUI(const GameState& gameState)
{
    Position mapSize = gameState.getMapSize();
    const auto& players = gameState.getPlayers();
    const auto& teams = gameState.getTeams();
    
    int alivePlayerCount = 0;
    for (const auto& pair : players) {
        if (pair.second.alive) alivePlayerCount++;
    }
    
    std::string info = "Map: " + std::to_string(mapSize.x) + "x" + std::to_string(mapSize.y) +
                      " | Players: " + std::to_string(alivePlayerCount) +
                      " | Teams: " + std::to_string(teams.size()) +
                      " | Time Unit: " + std::to_string(gameState.getTimeUnit()) +
                      "\nControls: WASD/Arrows - Move Camera | Mouse Wheel - Zoom | B - Menu | ESC - Quit";
    
    DrawTextEx(font, info.c_str(), {10, 10}, 16, 1, WHITE);
    
    float yOffset = 80;
    DrawTextEx(font, "Teams:", {10, yOffset}, 18, 1, WHITE);
    yOffset += 25;
    
    for (const auto& teamPair : teams) {
        const TeamData& team = teamPair.second;
        
        DrawRectangle(10, static_cast<int>(yOffset), 20, 15, team.color);
        DrawRectangleLines(10, static_cast<int>(yOffset), 20, 15, WHITE);
        
        int teamPlayers = 0;
        for (const auto& playerPair : players) {
            if (playerPair.second.alive && playerPair.second.teamName == team.name) {
                teamPlayers++;
            }
        }
        
        std::string teamInfo = team.name + " (" + std::to_string(teamPlayers) + " players)";
        DrawTextEx(font, teamInfo.c_str(), {35, yOffset}, 14, 1, WHITE);
        
        yOffset += 20;
    }
    
    std::string connectionStatus = "Status: Connected";
    DrawTextEx(font, connectionStatus.c_str(), 
        {(float)(GetScreenWidth() - 200), 10.0f}, 14, 1, GREEN);    
    if (gameState.isGameEnded()) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 128});        
        std::string winnerText = "Game Ended!\nWinner: " + gameState.getWinnerTeam();
        
        Vector2 textSize = MeasureTextEx(font, winnerText.c_str(), 48, 2);
        Vector2 position = {
            (GetScreenWidth() - textSize.x) / 2,
            (GetScreenHeight() - textSize.y) / 2
        };
        
        DrawTextEx(font, winnerText.c_str(), position, 48, 2, GOLD);
    }
}

void Renderer::handleMouseClick(const Vector2& mousePos, const Camera& camera, const GameState& gameState)
{
    Vector2 worldPos = camera.screenToWorld(mousePos);
    int tileX = static_cast<int>(worldPos.x / BASE_TILE_SIZE);
    int tileY = static_cast<int>(worldPos.y / BASE_TILE_SIZE);
    
    Position mapSize = gameState.getMapSize();
    if (tileX < 0 || tileX >= mapSize.x || tileY < 0 || tileY >= mapSize.y) {
        selectedObject.type = SelectedObject::NONE;
        return;
    }
    
    const TileData* tile = gameState.getTile(tileX, tileY);
    if (tile && !tile->players.empty()) {
        selectedObject.type = SelectedObject::PLAYER;
        selectedObject.position = Position(tileX, tileY);
        selectedObject.playerId = tile->players[0];
    } else {
        selectedObject.type = SelectedObject::TILE;
        selectedObject.position = Position(tileX, tileY);
        selectedObject.playerId = -1;
    }
}

}