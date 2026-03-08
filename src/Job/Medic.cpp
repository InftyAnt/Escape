#include "Medic.h"
#include <iostream>

// 생성자
Medic::Medic(Direction dir) : Player(dir, MEDIC) {
    firstCool = DEFAULT_MEDIC_FIRST_COOL * FPS;
    secondCool = 0;
    firstCoolCount = firstCool;
    secondCoolCount = 0;
}

// 첫 번째 스킬
void Medic::firstSkill() {
    if (firstCoolCount >= firstCool) {
        syringes.push_back(make_unique <Syringe>(direction, position, 25));
        firstCoolCount = 0;
    }
}

// 발사하고 효과가 사라진 주사기 제거
void Medic::deleteSyringe() {
    for (auto it = syringes.begin(); it != syringes.end(); ) {
        if (!(*it)->isVisible) {
            it = syringes.erase(it);
        }
        else {
            ++it;
        }
    }
}
