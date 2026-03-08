#include "Chairman.h"

// 생성자
Chairman::Chairman(Direction dir) : Player(dir, MEDIC) {
    firstCool = DEFAULT_CHAIRMAN_FIRST_COOL * FPS;
    secondCool = 0;
    firstCoolCount = firstCool;
    secondCoolCount = 0;

}

// 첫 번째 스킬
void Chairman::firstSkill() {
    if (firstCoolCount >= firstCool) {
        chairs.push_back(make_unique <Chair>(direction, position, 25));
        firstCoolCount = 0;
    }
}

// 휘두르고 효과가 사라진 의자 제거
void Chairman::deleteChair() {
    for (auto it = chairs.begin(); it != chairs.end(); ) {
        if (!(*it)->isVisible) {
            it = chairs.erase(it);
        }
        else {
            ++it;
        }
    }
}
