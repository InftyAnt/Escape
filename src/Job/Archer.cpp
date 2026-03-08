#include "Archer.h"
#include <iostream>

// 생성자
Archer::Archer(Direction dir) : Player(dir, ARCHER) {
    damage = 20;
    firstCool = DEFAULT_ARCHER_FIRST_COOL * FPS;
    secondCool = 0;
    firstCoolCount = firstCool;
    secondCoolCount = 0;
}

// 첫 번째 스킬
void Archer::firstSkill() {
    if (firstCoolCount >= firstCool) {
        arrows.push_back(make_unique <Arrow> (direction, position, getEffectiveDamage()));
        firstCoolCount = 0;
    }
}

// 발사하고 효과가 사라진 화살 제거
void Archer::deleteArrow() {
    for (auto it = arrows.begin(); it != arrows.end(); ) {
        if (!(*it)->isVisible) {
            it = arrows.erase(it);
        }
        else {
            ++it;
        }
    }
}