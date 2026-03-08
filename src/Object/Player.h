#pragma once

#include <utility>
#include <vector>
#include <stack>
#include <memory>
#include "../Main/Setting.h"
#include "../Map/Map.h"
#include "../Main/UI.h"
#include "../Item/Item.h"

class Player {
public :
	bool isAlive;												// 생존 여부
	bool isInvincible;											// 보이지 않는가 (공격받지 않는가, 무적시간 동안 활성화)
	int invincibleFrames;										// 남은 무적 시간
	const int invincibleDurationFrames = 2 * FPS;				// 공격받을 때 주어지는 무적 시간

	int health;													// 현재 체력
	int maxHealth;												// 최대 체력

	pair <int, int> position;									// 플레이어의 격자 위치
	pair <int, int> indirectPosition;
	ofVec2f actualPosition;										// 플레이어의 그리기 위치
	ofVec2f targetPosition;										// 플레이어의 타겟 위치
	float speed;												// 플레이어의 이동 속도
	Job job;													// 플레이어의 직업

	Direction direction;										// 플레이어의 방향
	int moveCount;												// 부드러운 움직임을 위한 중간 변수
	bool isMoving;												// 플레이어의 이동 여부
	int activeKey;												// 활성화된 키
	stack<int> keyStack;										// 활성화된 키 스택

	int firstCool;												// 첫 번째 스킬의 차지된 쿨타임 시간
	int secondCool;												// 첫 번째 스킬의 쿨타임 시간
	int firstCoolCount;											// 두 번째 스킬의 차지된 쿨타임 시간
	int secondCoolCount;										// 두 번째 스킬의 쿨타임 시간

	int healthDrawCount;										// 체력 표시 시간

	int damage;													// 플레이어의 공격 피해량

	int mineral;

	vector <unique_ptr <Item>> inventory;						// 플레이어의 인벤토리
	vector <pair <int, int>> activePowerUps;					// 플레이어의 액티브 공격력 강화 효과
	vector <pair <int, int>> activeSpeedUps;					// 플레이어의 액티브 이동 속도 강화 효과, 지금은 사용되지 않음
	bool isEPressed;
	
	Player(Direction dir, Job j);								// 생성자
	virtual ~Player();											// 소멸자
	void updateTargetPosition();								// 타겟 위치 업데이트
	void updateIndirectPosition();
	bool canMoveTo(int x, int y, const Room& room) const;		// 이동 가능 확인
	void playerMove(const Room& room);							// 플레이어의 이동
	void startMove(int key);									// 이동 시작
	void finishMove();											// 이동 종료

	virtual void keyPressed(int key);							// 키 입력 감지
	virtual void keyReleased(int key);							// 키 입력 해제 감지

	virtual void firstSkill();									// 첫 번째 스킬
	virtual void secondSkill();									// 두 번째 스킬
	virtual void updateCoolCount();								// 스킬의 쿨타임 업데이트

	void telePort(int r, int c);								// 플레이어의 위치 강제 이동
	
	virtual void draw(UI& ui);									// 플레이어 그리기
	void updateInvincible();									// 플레이어의 무적시간 업데이트
	virtual void update(const Room& room);						// 플레이어 업데이트
	void drawHealth();											// 플레이어의 체력바 그리기

	int getEffectiveDamage() const;								// 플레이어의 액티브 공격력 강화 설정
	void increaseDamage(int percentage, int durationInFrames);	// 플레이어의 공격력 강화 효과 추가
	void updateDamage();										// 플레이어의 공격력 업데이트

	bool addItem(unique_ptr <Item> item);						// 플레이어의 인벤토리에 아이템 추가
	void useItem(int index);									// 인벤토리에서 아이템 사용

};