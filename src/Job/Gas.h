#pragma once

#include "../Main/Setting.h"
#include "ofMain.h"
#include <vector>
#include <utility>

// 순환 종속성 방지를 위한 전방 선언
class GameManager;

// 파이어맨이 분사하는 가스
class Gas {
public:
	bool isVisible;										// 가스가 유효한가
	bool isAttacked;									// 가스의 타격 여부 (다중 타격 방지)
	pair <int, int> position;							// 가스의 기준 위치
	int damage;											// 가스의 피해량
	Direction direction;								// 가스의 공격 방향
	int animationCount;									// 가스의 애니메이션 시간
	vector <pair <int, int>> hitRange;					// 가스가 타격하는 좌표들

	Gas(Direction dir, pair <int, int> pos, int dmg);	// 생성자

	void move(const GameManager& gameManager);			// 가스 이동 (공격)
	void draw();										// 가스 그리기

	vector <pair<int, int>> getAttackRange();			// hitRange 계산
};