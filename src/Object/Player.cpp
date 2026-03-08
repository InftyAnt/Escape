#include "Player.h"

// 생성자
Player::Player(Direction dir, Job j) {
    isAlive = true;
    health = maxHealth = 250;
    job = j;
    direction = dir;
    speed = DEFAULT_PLAYER_SPEED;
    moveCount = 0;
    isMoving = false;
    activeKey = -1;
    position = { ROOM_SIZE / 2, ROOM_SIZE / 2 };
    actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
    targetPosition = actualPosition;
    damage = 0;
    isEPressed = false;

    firstCool = 0;
    firstCoolCount = 0;
    secondCool = 0;
    secondCoolCount = 0;

    isInvincible = false;
    invincibleFrames = 0;

    healthDrawCount = 0;

    mineral = 0;
}

// 소멸자
Player::~Player() {
    inventory.clear();
}

// 타겟 위치 업데이트
void Player::updateTargetPosition() {
    targetPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

void Player::updateIndirectPosition() {
    indirectPosition = position;
}

// 이동 가능 확인
bool Player::canMoveTo(int x, int y, const Room& room) const {
    return x >= 0 && x < room.width && y >= 0 && y < room.height && !room.roomState[x][y];
}

// 플레이어의 이동
void Player::playerMove(const Room& room) {
    // 만약 타겟 위치에 도착하기 전이라면 이동 키 입력 여부에 관계없이 타겟 위치까지 이동
    if (actualPosition != targetPosition) {
        ofVec2f directionVec = targetPosition - actualPosition;
        directionVec.normalize();
        actualPosition += directionVec * speed / FPS * CELL_SIZE;

        if ((targetPosition - actualPosition).length() < speed / FPS * CELL_SIZE) {
            actualPosition = targetPosition;
            indirectPosition = position;
        }
    }
    // 그렇지는 않지만 방향키 입력을 받았다면 그 방향으로 new 좌표 설정 후 타겟 위치 설정
    else if (activeKey != -1) {
        int newX = position.first;
        int newY = position.second;

        if (activeKey == OF_KEY_UP) {
            direction = Up;
            newY--;
        }
        else if (activeKey == OF_KEY_DOWN) {
            direction = Down;
            newY++;
        }
        else if (activeKey == OF_KEY_LEFT) {
            direction = Left;
            newX--;
        }
        else if (activeKey == OF_KEY_RIGHT) {
            direction = Right;
            newX++;
        }

        if (canMoveTo(newX, newY, room)) {
            position.first = newX;
            position.second = newY;
            updateTargetPosition();
            indirectPosition = { newX, newY };
        }

        isEPressed = false;
    }
}

// 이동 시작
void Player::startMove(int key) {
    // 키가 방향키인데 키 스택의 탑과 다른 키라면 키 스택에 넣음
    if (key == OF_KEY_UP || key == OF_KEY_DOWN || key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {
        if (keyStack.empty() || key != keyStack.top()) {
            keyStack.push(key);
        }
        activeKey = key;
        isMoving = true;
    }
}

// 이동 종료
void Player::finishMove() {
    isMoving = false;
    activeKey = -1;
}

// 키 입력 감지
void Player::keyPressed(int key) {
    if (key >= '0' && key <= '9') {
        int index = (key - '0' + 9) % 10;
        useItem(index);
    }
    else if (key == 'q') {
        firstSkill();
    }
    else if (key == 'w') {
        secondSkill();
    }
    else if (key == 'e') {
        isEPressed = true;
    }
    else {
        startMove(key);
    }
}

// 키 입력 해제 감지
void Player::keyReleased(int key) {

    // 해제한 키가 방향키이면
    if (key == OF_KEY_UP || key == OF_KEY_DOWN || key == OF_KEY_LEFT || key == OF_KEY_RIGHT) {

        // 해제한 키가 키 스택의 탑과 같다면 제거
        if (!keyStack.empty() && keyStack.top() == key) {
            keyStack.pop();

            // 키를 해제했음에도 다른 키 입력이 남아있으면 그 키에 대한 행동 활성화
            if (!keyStack.empty()) {
                activeKey = keyStack.top();
            }
            // 아니면 이동 종료 및 타겟 위치 업데이트
            else {
                activeKey = -1;
                finishMove();
            }
        }
        // 방향키 외의 키를 해제한 경우 처리
        else if (key == 'e') {
            isEPressed = false;
        }
        // 해제한 키가 키 스택의 탑이 아닌 경우
        else {
            // 임시 스택 선언
            stack<int> tempStack;

            // 스택에서 해제한 키를 찾기 위해 임시 스택으로 옮김
            while (!keyStack.empty() && keyStack.top() != key) {
                tempStack.push(keyStack.top());
                keyStack.pop();
            }

            // 키 스택에서 해제한 키를 제거
            if (!keyStack.empty()) {
                keyStack.pop();
            }

            // 임시 스택의 키를 원래 스택으로 다시 옮김
            while (!tempStack.empty()) {
                keyStack.push(tempStack.top());
                tempStack.pop();
            }

            // 모든 키가 해제되었을 때 이동 종료 및 타겟 위치 업데이트
            if (keyStack.empty()) {
                activeKey = -1;
                finishMove();
                updateTargetPosition();
            }
        }
    }
}

// 첫 번째 스킬
void Player::firstSkill() {

}

// 두 번째 스킬
void Player::secondSkill() {

}

// 스킬의 쿨타임 업데이트
void Player::updateCoolCount() {
    if (firstCoolCount < firstCool) firstCoolCount++;
    if (secondCoolCount < secondCool) secondCoolCount++;
}

// 플레이어의 위치 강제 이동
void Player::telePort(int r, int c) {
    position = { r, c };
    actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

// 플레이어 그리기
void Player::draw(UI& ui) {
    bool shouldDraw = true;

    // 무적 시간동안 깜빡거림
    if (isInvincible) {
        int blinkFrame = FPS / 10;
        shouldDraw = (invincibleFrames / blinkFrame) % 2 == 0;
    }
    if (shouldDraw) {
        int DirectionLineLength = CELL_SIZE / 2 + DIRECTION_LINE_WIDTH;
        int Remnant = CELL_SIZE / 2 - DIRECTION_LINE_WIDTH;

        // 플레이어가 보고 있는 방향에 따라 플레이어의 색을 바꿈, 바라보는 방향의 막대기도 바꿈
        switch (direction) {
        case Up:
            ofSetColor(255, 255, 0);
            ofDrawRectangle(actualPosition, CELL_SIZE, CELL_SIZE);
            ofSetColor(ofColor::black);
            ofDrawRectangle(actualPosition + ofVec2f(Remnant, 0), 2 * DIRECTION_LINE_WIDTH, DirectionLineLength);
            break;
        case Down:
            ofSetColor(0, 255, 255);
            ofDrawRectangle(actualPosition, CELL_SIZE, CELL_SIZE);
            ofSetColor(ofColor::black);
            ofDrawRectangle(actualPosition + ofVec2f(Remnant, Remnant), 2 * DIRECTION_LINE_WIDTH, DirectionLineLength);
            break;
        case Left:
            ofSetColor(ofColor::pink);
            ofDrawRectangle(actualPosition, CELL_SIZE, CELL_SIZE);
            ofSetColor(ofColor::black);
            ofDrawRectangle(actualPosition + ofVec2f(0, Remnant), DirectionLineLength, 2 * DIRECTION_LINE_WIDTH);
            break;
        case Right:
            ofSetColor(ofColor::white);
            ofDrawRectangle(actualPosition, CELL_SIZE, CELL_SIZE);
            ofSetColor(ofColor::black);
            ofDrawRectangle(actualPosition + ofVec2f(Remnant, Remnant), DirectionLineLength, 2 * DIRECTION_LINE_WIDTH);
            break;
        }
    }

    // 피격 시 플레이어의 체력 바를 플레이어 위에 그리기
    if (healthDrawCount > 0) {
        ofSetColor(127);
        ofDrawRectangle(actualPosition.x, actualPosition.y + ENTITY_HEALTHBAR_Y, CELL_SIZE, ENTITY_HEALTHBAR_W + ENTITY_HEALTHBAR_STROKE_W * 2);
        ofSetColor(ofColor::red);
        ofDrawRectangle(actualPosition.x + ENTITY_HEALTHBAR_STROKE_W, actualPosition.y + ENTITY_HEALTHBAR_Y + ENTITY_HEALTHBAR_STROKE_W, (CELL_SIZE - ENTITY_HEALTHBAR_STROKE_W * 2)* health / maxHealth, ENTITY_HEALTHBAR_W);
        ofSetColor(255);
        healthDrawCount--;
    }
}

// 플레이어의 무적시간 업데이트
void Player::updateInvincible() {
    if (isInvincible) {
        invincibleFrames++;
        if (invincibleFrames >= invincibleDurationFrames) {
            isInvincible = false;
            invincibleFrames = 0;
        }
    }
}

// 플레이어 업데이트
void Player::update(const Room& room) {
    playerMove(room);
    updateInvincible();
    updateCoolCount();
    updateDamage();
}

// 플레이어의 체력바 그리기
void Player::drawHealth() {
    healthDrawCount = ENTITY_HEALTH_DRAW_TIME;
}

// 플레이어의 액티브 공격력 강화 설정
int Player::getEffectiveDamage() const {
    int effectiveDamage = damage;
    for (const auto& powerUp : activePowerUps) {
        effectiveDamage += powerUp.second;
    }
    return effectiveDamage;
}

// 플레이어의 공격력 강화 효과 추가
void Player::increaseDamage(int percentage, int durationInFrames) {
    int addedDamage = damage * percentage / 100;
    activePowerUps.push_back({ durationInFrames, addedDamage });
}

// 플레이어의 공격력 업데이트
void Player::updateDamage() {
    auto it = remove_if(activePowerUps.begin(), activePowerUps.end(), [this](auto& powerUp) {
        if (--powerUp.first <= 0) {
            return true;
        }
        return false;
        });
    activePowerUps.erase(it, activePowerUps.end());
}

// 플레이어의 인벤토리에 아이템 추가
bool Player::addItem(unique_ptr<Item> item) {
    if (item->itemType == MINERAL) {
        item->use(*this);
        return true;
    }
    else if (inventory.size() < INVENTORY_SIZE) {
        inventory.push_back(move(item));
        return true;
    }
    else {
        return false;   
    }
}

// 인벤토리에서 아이템 사용
void Player::useItem(int index) {
    if (index < inventory.size() && inventory[index]) {
        inventory[index]->use(*this);
        if (inventory[index]->itemType == CONSUMABLE) {
            inventory.erase(inventory.begin() + index);
        }
    }
}