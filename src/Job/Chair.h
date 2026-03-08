#pragma once

#include "../Main/Setting.h"
#include "ofMain.h"
#include <vector>
#include <utility>

// 순환 종속성 방지를 위한 전방 선언
class GameManager;

// 체어맨이 휘두르는 의자
class Chair {
public:
	bool isVisible;											// 의자가 유효한가
	bool isAttacked;										// 의자의 타격 여부 (다중 타격 방지)
	pair <int, int> position;								// 의자의 기준 위치
	int damage;												// 의자의 피해량
	Direction direction;									// 의자의 공격 방향
	int animationCount;										// 의자의 애니메이션 시간
	vector <pair <int, int>> hitRange;						// 의자가 타격하는 좌표들

	Chair(Direction dir, pair <int, int> pos, int dmg);		// 생성자

	void move(const GameManager& gameManager);				// 의자 이동 (공격)
	void draw();											// 의자 그리기

	vector <pair<int, int>> getAttackRange();				// hitRange 계산
};