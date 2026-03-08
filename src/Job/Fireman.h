#pragma once

#include "../Main/Setting.h"
#include "../Map/Room.h"
#include "../Object/Player.h"
#include "Gas.h"
#include <vector>

// 순환 종속성 방지를 위한 전방 선언
class GameManager;
 
// 파이어맨 : 플레이어의 직업 중 하나, 가스를 분사하여 공격한다.
class Fireman : public Player {
public:
    vector <unique_ptr <Gas>> gas;              // 분사한 가스들
    bool isAttacking;                           // 공격 중인가
    int hiddenCool;                             // 가스가 공격하는 주기
    int hiddenCoolCount;                        // 가스 공격 주기마다 공격하도록 하기 위한 중간 변수
    int drawCool;                               // 가스 그리기 쿨타임

    Fireman(Direction dir);                     // 생성자

    void firstSkill();                          // 첫 번째 스킬

    void deleteGas();                           // 분사하고 효과가 사라진 가스 제거

    void keyPressed(int key) override;          // 키 입력 감지 재정의
    void keyReleased(int key) override;         // 키 입력 해제 감지 재정의
    void update(const Room& room) override;     // 업데이트
};
