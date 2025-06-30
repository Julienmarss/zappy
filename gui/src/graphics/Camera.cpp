#include "../../include/Camera.hpp"

namespace Zappy {

Camera::Camera()
{
    raylibCamera.target = {0, 0};
    raylibCamera.offset = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    raylibCamera.rotation = 0.0f;
    raylibCamera.zoom = 1.0f;
    
    zoomLevel = 1.0f;
    targetPosition = Position(0, 0);
    smoothMovement = true;
}

void Camera::update()
{
    if (smoothMovement) {
        float lerpFactor = 5.0f * GetFrameTime();
        raylibCamera.target.x += (targetPosition.x - raylibCamera.target.x) * lerpFactor;
        raylibCamera.target.y += (targetPosition.y - raylibCamera.target.y) * lerpFactor;
    } else {
        raylibCamera.target.x = static_cast<float>(targetPosition.x);
        raylibCamera.target.y = static_cast<float>(targetPosition.y);
    }
    
    raylibCamera.zoom = zoomLevel;
}

void Camera::setTarget(const Position& target)
{
    targetPosition = target;
}

void Camera::move(const Vector2& offset)
{
    targetPosition.x += static_cast<int>(offset.x);
    targetPosition.y += static_cast<int>(offset.y);
}

void Camera::zoom(float zoomDelta)
{
    zoomLevel += zoomDelta;
    
    if (zoomLevel < 0.1f) zoomLevel = 0.1f;
    if (zoomLevel > 5.0f) zoomLevel = 5.0f;
}

Vector2 Camera::screenToWorld(const Vector2& screenPos) const
{
    return GetScreenToWorld2D(screenPos, raylibCamera);
}

Vector2 Camera::worldToScreen(const Vector2& worldPos) const
{
    return GetWorldToScreen2D(worldPos, raylibCamera);
}

}