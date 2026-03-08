#include "Map.h"

// 생성자
Map::Map(int minRooms, int maxRooms) {
	roomCnt = randInt(minRooms, maxRooms);
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			rooms[i][j] = make_unique <Room> (i, j);
		}
	}
	
	// 포탈 방 설정
	int r_p = randInt(MAP_SIZE - 1);
	int c_p = randInt(MAP_SIZE - 1);
	portal = rooms[r_p][c_p].get();
	portal->isPortal = true;
	portal->isIncluded = true;
	portal->isClear = true;
	included.push_back(portal);
	addToNearby(r_p, c_p);

	// 나머지 방들을 연결
	for (int i = 0; i < roomCnt - 1; i++) includeRandomRoom(i);

	// 방들 기본 설정
	setRooms();
	setAvailable();
	setRoomWalls();
	setRoomDoors();
	setRoomZombies();
}

// 소멸자
Map::~Map() {

}

// 좌표의 방 기준 해당 방향의 방
Room* Map::where(int r, int c, int dir) {
	return rooms[r + dr[dir]][c + dc[dir]].get();
}

// 좌표의 유효성
bool Map::isRangeValid(int i, int j) {
	return i >= 0 && i < MAP_SIZE && j >= 0 && j < MAP_SIZE;
}

// 좌표 기준 해당 방향 좌표의 유효성
bool Map::isRangeValid(int i, int j, int dir) {
	return isRangeValid(i + dr[dir], j + dc[dir]);
}

// 좌표 기준 해당 방향 좌표의 유효성
bool Map::isRangeValid(int i, int j, Direction dir) {
	return isRangeValid(i + dr[dir], j + dc[dir]);
}

// 임의의 방을 included에 넣음
void Map::includeRandomRoom(int cnt) {

	// included는 현재 포함시킨 방들, nearby는 included과 연결되어 있으나 included에는 포함되지 않는 방들
	// nearby에서 임의의 방 하나를 골라 included에 추가함.
	int idx = randInt(nearby.size() - 1);
	included.push_back(nearby[idx]);
	included.back()->isIncluded = true;
	included.back()->isNear = false;

	//nearby에서 방금 추가한 방을 뒤로 뺀 후 제거
	nearby[idx] = nearby.back();
	nearby.pop_back();

	int r = included.back()->row;
	int c = included.back()->col;

	// 만약 지금 방이 보스 방이거나 보스 방과 붙어 있는 방이면 연결 방식이 일반 방과 다르므로 nearby 초기화
	if (cnt == 0 || cnt == 1) {
		for (Room* room : nearby) {
			room->isNear = false;
		}
		nearby.clear();
	}

	// nearby에 방금 추가한 방에 대한 주변 방들을 추가로 넣음
	addToNearby(r, c);

	// 방금 추가한 방과 기존에 추가했던 방들을 모두 연결
	connect(r, c, cnt);

	// 보스 방이라면 보스 방 설정
	if (cnt == 0) {
		boss = rooms[r][c].get();
		boss->isBoss = true;
	}
}

// nearby에 방을 넣음
void Map::addToNearby(int r, int c) {
	for (int i = 0; i < DIRECTION_COUNT; i++) {
		if (isRangeValid(r, c, i) && !where(r, c, i)->isIncluded && !where(r, c, i)->isNear) {
			nearby.push_back(where(r, c, i));
			where(r, c, i)->isNear = true;
		}
	}
}

// 방을 주위의 방들과 연결함
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

// 맵의 방들을 총 설정
void Map::setRooms() {
	// 보물 방과 상점 방은 구현하지 않았으므로 시작 방 설정만 진행함

	shop.resize(2);
	vector<Room*> availables(included.begin() + 2, included.end());
	set <Room*> chosen;
	int cnt = roomCnt - 2;
	start = chooseUniqueRoom(availables, chosen);
	start->isStart = true;
	start->isClear = true;

	treasure.push_back(chooseUniqueRoom(availables, chosen));
	shop[0] = chooseUniqueRoom(availables, chosen);
	shop[1] = chooseUniqueRoom(availables, chosen);
	if(roomCnt == MAX_ROOM_CNT) treasure.push_back(chooseUniqueRoom(availables, chosen));
	for (auto t : treasure) {
		t->isTreasure = true;
	}
	for (auto s : shop) {
		s->isShop = true;
	}
}

// 맵의 방들 중 접근 가능한 방들 설정
void Map::setAvailable() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			for (int k = 0; k < DIRECTION_COUNT; k++) rooms[i][j]->isAvailable |= rooms[i][j]->neighbor[k];
		}
	}
}

// 다른 방들과 연결되는 문 설정
void Map::setRoomDoors() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (rooms[i][j]->isAvailable) rooms[i][j]->setDoors();
		}
	}
}

// 각 방들의 벽 설정
void Map::setRoomWalls() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (rooms[i][j]->isAvailable) rooms[i][j]->setWalls();
		}
	}
}

// 각 방들의 좀비 설정
void Map::setRoomZombies() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (rooms[i][j]->isAvailable) rooms[i][j]->setZombies();
		}
	}
}

// 겹치지 않는 임의의 접근 가능한 방
Room* Map::chooseUniqueRoom(vector<Room*>& rooms, set<Room*>& chosen) {
	while (true) {
		int idx = randInt(rooms.size() - 1);
		if (chosen.find(rooms[idx]) == chosen.end()) {
			chosen.insert(rooms[idx]);
			return rooms[idx];
		}
	}
	return nullptr;
}