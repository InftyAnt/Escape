#pragma once

#include "Setting.h"
#include "ofMain.h"
#include <string>

// 순환 종속성 방지를 위한 전방 선언
class GameManager;
class Player;
class Zombie;
class Map;

// 유저 인터페이스
class UI {
public :
	ofTrueTypeFont font15;												// 폰트
	ofTrueTypeFont fontb15;

	UI();																// 생성자
	~UI();																// 소멸자
	void drawMinimap(const GameManager& gameManager, const Map& map);	// 미니맵 그리기
	void drawHealthBar(const Player& player);							// 플레이어의 체력바 그리기
	void drawCool(const Player& player);								// 스킬의 쿨타임 그리기
	void drawInventory(const Player& player);							// 플레이어의 인벤토리 그리기
	void drawMineral(const Player& playeer);
	void showRoomZombieState(const GameManager& gameManager);
};