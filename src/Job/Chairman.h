#pragma once
#include "../Object/Player.h"
#include "Chair.h"

// 체어맨 : 플레이어의 직업 중 하나, 의자를 휘둘러 공격한다.
class Chairman : public Player {
public :
    vector <unique_ptr <Chair>> chairs;     // 휘두르는 의자들

    Chairman(Direction dir);                // 생성자

    void firstSkill() override;             // 첫 번째 스킬
    void deleteChair();                     // 휘두르고 효과가 사라진 의자 제거
};