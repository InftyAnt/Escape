#include "Camera.h"
#include "../Object/Player.h"

// 생성자
Camera::Camera(int w, int h, int mw, int mh)
    : x(0), y(0), width(w), height(h), roomWidth(mw), roomHeight(mh) {}

// 카메라 업데이트
void Camera::update(const Player& player) {
    x = player.actualPosition.x + CELL_SIZE / 2 - width / 2;
    y = player.actualPosition.y + CELL_SIZE / 2 - height / 2;

    // 카메라가 방의 범위를 벗어난 부분을 찍지 않도록 제한함
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > roomWidth - width) x = roomWidth - width;
    if (y > roomHeight - height) y = roomHeight - height;
}