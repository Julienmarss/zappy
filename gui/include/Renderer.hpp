/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** Renderer header
*/

#pragma once
#include "Common.hpp"

namespace Zappy {
    class GameState;
    class Camera;

    class Renderer {
    public:
        struct SelectedObject {
            enum Type { NONE, TILE, PLAYER } type = NONE;
            Position position{0, 0};
            int playerId = -1;
        };

        Renderer() = default;
        ~Renderer();

        bool loadResources();
        void renderFrame(const GameState& gameState, const Camera& camera);
        
        void handleMouseClick(const Vector2& mousePos, const Camera& camera, const GameState& gameState);
        const SelectedObject& getSelectedObject() const { return selectedObject; }

    private:
        Font font{};
        bool resourcesLoaded{false};
        
        SelectedObject selectedObject;

        void renderMap(const GameState& gameState, const Camera& camera);
        void renderTile(const TileData& tile, int x, int y);
        void renderPlayers(const GameState& gameState);
        void renderUI(const GameState& gameState);
        void renderTileResources(const TileData& tile, float tileX, float tileY);

        Color getTeamColor(const std::string& teamName) const;
        Color getResourceColor(ResourceType type) const;
        const char* getResourceName(ResourceType type) const;
    };
}