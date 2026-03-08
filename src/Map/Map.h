#pragma once

#include <vector>
#include "Room.h"

// 맵 : 방들의 배치를 결정
class Map {
public :
	int roomCnt;															// 방의 개수
	unique_ptr<Room> rooms[MAP_SIZE][MAP_SIZE];								// 방들의 배열
	vector <Room*> included;												// 실제 존재하는 방들
	vector <Room*> nearby;													// 방들 사이 연결 관계를 설정하기 위한 중간 벡터
	Room* portal;															// 포탈 방
	Room* boss;																// 보스 방
	Room* start;															// 시작 방
	vector <Room*> treasure;
	vector <Room*> shop;

	Map(int minRooms, int maxRooms);										// 생성자
	~Map();																	// 소멸자

	Room* where(int r, int c, int dir);										// 좌표의 방 기준 해당 방향의 방
	bool isRangeValid(int i, int j);										// 좌표의 유효성
	bool isRangeValid(int i, int j, int dir);								// 좌표 기준 해당 방향 좌표의 유효성
	bool isRangeValid(int i, int j, Direction dir);							// 좌표 기준 해당 방향 좌표의 유효성
	void includeRandomRoom(int cnt);										// 임의의 방을 included에 넣음
	void addToNearby(int r, int c);											// nearby에 방을 넣음
	void connect(int r, int c, int cnt);									// 방을 주위의 방들과 연결함
	void setRooms();														// 맵의 방들을 총 설정
	void setAvailable();													// 맵의 방들 중 접근 가능한 방들 설정
	void setRoomDoors();													// 다른 방들과 연결되는 문 설정
	void setRoomWalls();													// 각 방들의 벽 설정
	void setRoomZombies();													// 각 방들의 좀비 설정
	Room* chooseUniqueRoom(vector <Room*>& rooms, set <Room*>& chosen);		// 겹치지 않는 임의의 접근 가능한 방

};