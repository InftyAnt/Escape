#pragma once

#include <vector>
#include "Setting.h"
#include "Arrow.h"
#include "Player.h"
#include "Archer.h"
#include "Map.h"
#include "Camera.h"
#include "UI.h"

class GameManager {
public :
	Player player;
	Camera camera;
	Map map;
	Room* currRoom;
	UI ui;
	ofFbo fbo;

	GameManager();
	~GameManager();

	void update();
	void draw();

	void changeRoom();
};