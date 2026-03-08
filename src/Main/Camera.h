#pragma once

#include "ofMain.h"
#include "Setting.h"

// 순환 종속성 방지를 위한 전방 선언
class Player;

class Camera {
public:
    int x, y;                               // 카메라가 촬영하는 부분의 가장 왼쪽 위 좌표
    int width, height;                      // 카메라가 촬영하는 부분의 가로, 세로 폭
    int roomWidth, roomHeight;              // 카메라가 촬영하는 방의 가로, 세로 폭

    Camera(int w, int h, int mw, int mh);   // 생성자
    void update(const Player& player);      // 카메라 업데이트
};