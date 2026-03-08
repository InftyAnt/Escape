#include "Map.h"

//--------------------------------------------------------------
Map::Map(int minRooms, int maxRooms) {
	roomCnt = rand() % (maxRooms - minRooms + 1) + minRooms;
	srand(time(0));
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			rooms[i][j] = make_unique <Room> (i, j);
		}
	}
	for (int i = 0; i < 2 * MAP_SIZE + 1; i++) {
		for (int j = 0; j < 2 * MAP_SIZE + 1; j++) {
			roomsState[i][j] = 0;
		}
	}
	int r_p = rand() % MAP_SIZE;
	int c_p = rand() % MAP_SIZE;
	portal = rooms[r_p][c_p].get();
	portal->isPortal = true;
	portal->isIncluded = true;
	included.push_back(portal);
	addToNearby(r_p, c_p);
	for (int i = 0; i < roomCnt - 1; i++) includeRandomRoom(i);
	setStart();
	setAvailable();
	setRoomWalls();
	setRoomDoors();
}

//--------------------------------------------------------------
Map::~Map() {

}

//--------------------------------------------------------------
Room* Map::where(int r, int c, int dir) {
	return rooms[r + dr[dir]][c + dc[dir]].get();
}

//--------------------------------------------------------------
bool Map::isRangeValid(int i, int j) {
	return i >= 0 && i < MAP_SIZE && j >= 0 && j < MAP_SIZE;
}

//--------------------------------------------------------------
bool Map::isRangeValid(int i, int j, int dir) {
	return isRangeValid(i + dr[dir], j + dc[dir]);
}

//--------------------------------------------------------------
bool Map::isRangeValid(int i, int j, Direction dir) {
	return isRangeValid(i + dr[dir], j + dc[dir]);
}

//--------------------------------------------------------------
void Map::includeRandomRoom(int cnt) {
	int idx = rand() % nearby.size();
	included.push_back(nearby[idx]);
	included.back()->isIncluded = true;
	included.back()->isNear = false;
	nearby[idx] = nearby.back();
	nearby.pop_back();
	int r = included.back()->row;
	int c = included.back()->col;
	if (cnt == 0 || cnt == 1) {
		for (Room* room : nearby) {
			room->isNear = false;
		}
		nearby.clear();
	}
	addToNearby(r, c);
	connect(r, c, cnt);
	if (cnt == 0) {
		boss = rooms[r][c].get();
		boss->isBoss = true;
	}
}

//--------------------------------------------------------------
void Map::addToNearby(int r, int c) {
	for (int i = 0; i < DIRECTION_COUNT; i++) {
		if (isRangeValid(r, c, i) && !where(r, c, i)->isIncluded && !where(r, c, i)->isNear) {
			nearby.push_back(where(r, c, i));
			where(r, c, i)->isNear = true;
		}
	}
}

//--------------------------------------------------------------
void Map::connect(int r, int c, int cnt) {
	for (int i = 0; i < DIRECTION_COUNT; i++) {
		if (isRangeValid(r, c, i) && where(r, c, i)->isIncluded) {
			bool P = where(r, c, i)->isPortal, B = where(r, c, i)->isBoss;
			if ((!P && !B) || (cnt == 0 && P && !B) || (cnt == 1 && !P && B)) {
				rooms[r][c]->neighbor[i] = true;
				where(r, c, i)->neighbor[(i + 2) % DIRECTION_COUNT] = true;
			}
		}
	}
}

//--------------------------------------------------------------
void Map::setStart() {
	int idx = rand() % (roomCnt - 2) + 2;
	start = included[idx];
	start->isStart = true;
}

//--------------------------------------------------------------
void Map::setAvailable() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			for (int k = 0; k < DIRECTION_COUNT; k++) rooms[i][j]->isAvailable |= rooms[i][j]->neighbor[k];
		}
	}
}

void Map::setRoomDoors() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (rooms[i][j]->isAvailable) rooms[i][j]->setDoors();
		}
	}
}

void Map::setRoomWalls() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (rooms[i][j]->isAvailable) rooms[i][j]->setWalls();
		}
	}
}