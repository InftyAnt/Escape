#pragma once

#include <utility>
#include <vector>
#include <stack>
#include "Setting.h"
#include "Arrow.h"
#include "Map.h"

typedef enum {
	NONE,
	ARCHER,
	MEDIC,
	CHAIRMAN,
	FIREMAN,
	BOMBER,
	BACKATTACKER
} Job;

class Player {
public :
	bool isAlive;
	int health;
	pair <int, int> position;
	ofVec2f actualPosition;
	ofVec2f targetPosition;
	float speed;
	Job job;

	Direction direction;
	int moveCount;
	bool isMoving;
	int activeKey;
	stack<int> keyStack;
	
	Player(Direction dir, Job j);
	~Player();
	void updateTargetPosition();
	bool canMoveTo(int x, int y, const Room& room) const;
	void move(const Room& room);
	void startMove(int key);
	void finishMove();

	void keyPressed(int key);
	void keyReleased(int key);

	virtual void firstSkill();
	virtual void secondSkill();

	void telePort(int r, int c);
	
	void draw();
};