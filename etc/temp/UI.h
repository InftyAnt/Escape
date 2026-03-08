#pragma once

#include "Setting.h"
#include "Map.h"

class GameManager;

class UI {
public :
	UI();
	~UI();
	void drawMinimap(const GameManager& gameManager, const Map& map);
};