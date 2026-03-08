#pragma once

#include <vector>
#include "Room.h"

class Map {
public :
	int roomCnt;
	unique_ptr<Room> rooms[MAP_SIZE][MAP_SIZE];
	vector <Room*> included;
	vector <Room*> nearby;
	Room* portal;
	Room* boss;
	Room* start;
	int roomsState[2 * MAP_SIZE + 1][2 * MAP_SIZE + 1];

	Map(int minRooms, int maxRooms);
	~Map();

	Room* where(int r, int c, int dir);
	bool isRangeValid(int i, int j);
	bool isRangeValid(int i, int j, int dir);
	bool isRangeValid(int i, int j, Direction dir);
	void includeRandomRoom(int cnt);
	void addToNearby(int r, int c);
	void connect(int r, int c, int cnt);
	void setStart();
	void setAvailable();
	void setRoomDoors();
	void setRoomWalls();
};