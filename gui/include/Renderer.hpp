#pragma once
#include "Common.hpp"

namespace Zappy {
    class GameState;
    class Camera;

    class Renderer {
    private:
        Font font{};
        bool resourcesLoaded{false};

        void renderMap(const GameState& gameState, const Camera& camera);
        void renderTile(const TileData& tile, int x, int y);
        void renderPlayers(const GameState& gameState);
        void renderUI(const GameState& gameState);
        void renderTileResources(const TileData& tile, float tileX, float tileY);

        Color getTeamColor(const std::string& teamName) const;
        Color getResourceColor(ResourceType type) const;
        const char* getResourceName(ResourceType type) const;

    public:
        Renderer() = default;
        ~Renderer();

        bool loadResources();
        void renderFrame(const GameState& gameState, const Camera& camera);
    };
}