#pragma once

#include "../Main/Setting.h"
#include "ofMain.h"
#include <utility>

// 순환 종속성 방지를 위한 전방 선언
class GameManager;

// 메딕이 발사하는 주사기
class Syringe {
public :
	bool isVisible;											// 주사기가 유효한가
	pair <int, int> position;								// 주사기의 격차 위치
	ofVec2f actualPosition;									// 주사기의 그리기 위치
	int damage;												// 주사기의 피해량
	float speed;											// 주사기의 발사 속도
	Direction direction;									// 주사기의 발사 방향
	int lifeCount;											// 주사기의 남은 유효 거리
	int moveCount;											// 부드러운 움직임을 위한 중간 변수
	
	Syringe(Direction dir, pair <int, int> pos, int dmg);	// 생성자

	void move(const GameManager& gameManager);				// 주사기 이동
	void draw();											// 주사기 그리기
};