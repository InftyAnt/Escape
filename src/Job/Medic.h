#pragma once

#include "../Main/Setting.h"
#include "../Object/Player.h"
#include "Syringe.h"

// 메딕 : 플레이어의 직업 중 하나, 주사기를 발사하여 공격하며 방 클리어 시 자가 회복한다.
class Medic : public Player {
public:
    vector <unique_ptr <Syringe>> syringes;     // 발사한 주사기들

    Medic(Direction dir);                       // 생성자

    void firstSkill() override;                 // 첫 번째 스킬
    void deleteSyringe();                       // 발사하고 효과가 사라진 주사기 제거
};