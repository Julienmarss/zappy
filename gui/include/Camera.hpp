#pragma once
#include "Common.hpp"

namespace Zappy {
    class Camera {
    private:
        Camera2D raylibCamera{};
        float zoomLevel{1.0f};
        Position targetPosition{0, 0};
        bool smoothMovement{true};

    public:
        Camera();
        ~Camera() = default;

        void update();
        void setTarget(const Position& target);
        void move(const Vector2& offset);
        void zoom(float zoomDelta);

        Vector2 screenToWorld(const Vector2& screenPos) const;
        Vector2 worldToScreen(const Vector2& worldPos) const;

        const Camera2D& getRaylibCamera() const { return raylibCamera; }
        float getZoom() const { return zoomLevel; }
        Position getTarget() const { return targetPosition; }
    };
}