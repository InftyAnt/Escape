#pragma once

#include "../Main/Setting.h"
#include "ofMain.h"
#include "../Object/Zombie.h"
#include <unordered_set>

// 순환 종속성 방지를 위한 전방 선언
class Map;
class GameManager;

// 아처가 발사하는 화살
class Arrow {
public:
	bool isVisible;											// 화살이 유효한가
	pair <int, int> position;								// 화살의 격자 위치
	ofVec2f actualPosition;									// 화살의 그리기 위치
	int damage;												// 화살의 피해량
	float speed;											// 화살의 발사 속도
	Direction direction;									// 화살의 발사 방향
	int lifeCount;											// 화살의 남은 유효 거리
	int moveCount;											// 부드러운 움직임을 위한 중간 변수
	unordered_set <Zombie*> hitZombies;						// 화살이 타격한 좀비들 (다중 타격 방지)

	Arrow(Direction dir, pair <int, int> pos, int dmg);		// 생성자

	void move(const GameManager& gameManager);				// 화살 이동
	void draw();											// 화살 그리기
};