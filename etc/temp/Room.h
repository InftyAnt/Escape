#pragma once

#include "ofMain.h"
#include "Setting.h"
#include "Direction.h"
#include "RoomType.h"
#include "Zombie.h"

class Room {
public :
	int row;
	int col;
	bool isAvailable;
	bool isIncluded;
	bool isNear;
	bool isStart;
	bool isBoss;
	bool isPortal;

	int width, height;
	vector <vector <int>> roomState;
	vector <pair <int, int>> doors;

	//vector <unique_ptr<Zombie>> zombies;


	vector <bool> neighbor;
	Room(int row, int col);
	~Room();

	void setCircle(vector <pair <int, int>>& circle, int radius);
	void setWalls();
	void setDoors();
	void setZombies();

	void draw();

	void f(int i, int j);
	void f(int posX, int posY, int i, int j);
	void f(int i, int j, int code);
	void f(int posX, int posY, int i, int j, int code);
};