#pragma once

#include "ofMain.h"
#include "Setting.h"

class Player;

class Camera {
public:
    int x, y;
    int width, height;
    int mapWidth, mapHeight;

    Camera(int w, int h, int mw, int mh);
    void update(const Player& player);
};