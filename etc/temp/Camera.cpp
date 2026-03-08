#include "Camera.h"
#include "Player.h"

Camera::Camera(int w, int h, int mw, int mh)
    : x(0), y(0), width(w), height(h), mapWidth(mw), mapHeight(mh) {}

void Camera::update(const Player& player) {
    x = player.actualPosition.x + CELL_SIZE / 2 - width / 2;
    y = player.actualPosition.y + CELL_SIZE / 2 - height / 2;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > mapWidth - width) x = mapWidth - width;
    if (y > mapHeight - height) y = mapHeight - height;
}