#pragma once

#include <memory>
#include <string>
#include "ofMain.h"
#include "../Main/Setting.h"
#include "../Main/UI.h"
#include "RoomType.h"
#include "TileType.h"
#include "../Object/Zombie.h"
#include "../Item/Item.h"

// 방
class Room {
public :
	int row;															// 행 번호
	int col;															// 열 번호
	bool isAvailable;													// 방 접근 가능성
	bool isClear;														// 방의 모든 좀비가 처치되었는가
	bool healthIncreased;												// 메딕의 경우 체력 충전이 되었는가
	bool isIncluded;													// 방이 Map의 included에 포함되었는가
	bool isNear;														// 방이 Map의 nearby에 포함되었는가
	bool isStart;														// 방이 시작 방인가
	bool isBoss;														// 방이 보스 방인가
	bool isPortal;														// 방이 포탈 방인가
	bool isTreasure;
	bool isShop;
	int zombieCnt;														// 방에 있는 좀비의 수

	int width, height;													// 방의 크기
	vector <vector <int>> roomState;									// 방의 벽, 문 등의 상태
	vector <pair <int, int>> doors;										// 방의 문 좌표들
	
	vector <unique_ptr<Zombie>> zombies;								// 방의 좀비들

	vector <bool> neighbor;												// 주변 방과 연결되어 있는가

	vector <pair <unique_ptr <Item>, pair <int, int>>> droppedItems;	// 방에 드랍된 아이템들

	vector <unique_ptr <Item>> shopItems;
	vector <pair <int, int>> shopItemPositions;

	Room(int row, int col);												// 생성자
	~Room();															// 소멸자

	void setCircle(vector <pair <int, int>>& circle, int radius);		// 원형 장애물 설정
	void setWalls();													// 벽 설정

	bool allZombiesDefeated() const;									// 방의 좀비가 모두 처치되었는지 확인
	void closeDoors();													// 방의 문을 닫음
	void openDoors(GameManager& gameManager);							// 방의 문을 엶

	void setDoors();													// 방의 문 위치 설정
	void setZombies();													// 방의 좀비 생성 및 위치 설정
	void deleteZombie();												// 방의 처치된 좀비 제거

	void draw(UI& ui);													// 방 그리기
	void drawItems();													// 방의 아이템들 그리기

	void f(int i, int j);												// 위치에 장애물 설치
	void f(int posX, int posY, int i, int j);							// 위치 기준 상대 위치에 장애물 설치
	void f(int i, int j, int code);										// 위치에 장애물 설치
	void f(int posX, int posY, int i, int j, int code);					// 위치 기준 상대 위치에 장애물 설치

	void drawShopItems();

};