#include "Fireman.h"

// 생성자
Fireman::Fireman(Direction dir) : Player(dir, FIREMAN) {
    secondCool = 0;
    firstCool = firstCoolCount = GAS_STORAGE;
    secondCoolCount = 0;
    isAttacking = false;
    hiddenCoolCount = hiddenCool = GAS_COOL * FPS;
    drawCool = 0;
}

// 첫 번째 스킬
void Fireman::firstSkill() {
    // 공격 중이지 않고 게이지량이 초기 소모량보다 많다면
    // 공격 중으로 바꾼 후 초기 소모량만큼 게이지 감소
    if (!isAttacking && firstCoolCount >= GAS_SACRIFICE) {
        isAttacking = true;
        firstCoolCount -= GAS_SACRIFICE;
        hiddenCoolCount = 0;
    }

    // 공격 중이고 공격 주기가 됐을 때
    if (isAttacking && hiddenCoolCount <= 0) {
        // 게이지가 충분하면 가스를 생성하면서 게이지 소모
        if (firstCoolCount >= GAS_DISCHARGE) {
            firstCoolCount -= GAS_DISCHARGE;
            hiddenCoolCount = hiddenCool;
            gas.push_back(make_unique <Gas> (direction, position, GAS_DAMAGE));
            drawCool = GAS_COOL * FPS;
        }
        // 그렇지 않으면 공격 취소
        else {
            isAttacking = false;
        }
    }
    // 공격 중인데 공격 주기는 아니면
    else if (isAttacking) {
        hiddenCoolCount--;
        // 게이지가 충분하면 그냥 게이지 소모
        if (firstCoolCount >= GAS_DISCHARGE) {
            firstCoolCount -= GAS_DISCHARGE;
        }
        // 그렇지 않으면 공격 취소
        else {
            isAttacking = false;
        }
    }

    // 공격 주기 갱신
    if (firstCoolCount < 0) {
        firstCoolCount = 0;
    }
}

// 분사하고 효과가 사라진 가스 제거
void Fireman::deleteGas() {
    for (auto it = gas.begin(); it != gas.end(); ) {
        if (!(*it)->isVisible) {
            it = gas.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 키 입력 감지
void Fireman::keyPressed(int key) {
    if (key == 'q') {
        if (firstCoolCount >= GAS_SACRIFICE) {
            firstSkill();
        }
    }
    else {
        Player::keyPressed(key);
    }
}

// 키 입력 해제 감지
void Fireman::keyReleased(int key) {
    if (key == 'q') {
        isAttacking = false;
        hiddenCoolCount = hiddenCool;
    }
    else {
        Player::keyReleased(key);
    }
}

// 업데이트
void Fireman::update(const Room& room) {
    playerMove(room);
    updateInvincible();
    if (isAttacking) {
        firstSkill();
    }
    else {
        if (firstCoolCount < GAS_STORAGE) {
            firstCoolCount += GAS_CHARGE;
        }
    }
    deleteGas();
    if (drawCool > 0) {
        drawCool--;
    }
}
