#pragma once

#include "Direction.h"
#include "Setting.h"
#include "ofMain.h"

class Camera;

#define ARROW_SPEED 16

class Arrow {
public:
	bool isVisible;
	pair <int, int> position;
	ofVec2f actualPosition;
	int damage;
	float speed;
	Direction direction;
	int lifeCount;
	int moveCount;

	Arrow(Direction dir, std::pair <int, int> pos, int dmg);
	~Arrow();

	void move();
	void draw(const Camera& camera) const;
};