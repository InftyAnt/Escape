#pragma once

#include "../Main/Setting.h"
#include "../Object/Player.h"
#include "Arrow.h"
#include <vector>
#include <utility>

// 아처 : 플레이어의 직업 중 하나, 화살을 발사하여 공격한다.
class Archer : public Player {
public:
    vector <unique_ptr <Arrow>> arrows; // 발사한 화살들

    Archer(Direction dir);              // 생성자

    void firstSkill() override;         // 첫 번째 스킬
    void deleteArrow();                 // 발사하고 효과가 사라진 화살 제거
};
