#include "Syringe.h"
#include "../Main/GameManager.h"

// 생성자
Syringe::Syringe(Direction dir, pair <int, int> pos, int dmg) {
	isVisible = true;
	direction = dir;
	position = pos;
	speed = SYRINGE_SPEED;
	damage = dmg;
	lifeCount = 5;
	moveCount = 0;
	actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

// 주사기 이동
void Syringe::move(const GameManager& gameManager) {
    if (lifeCount == 0) {
        isVisible = false;
        return;
    }

    // moveDistance는 프레임 당 이동량. moveCount에 이 값을 프레임마다 더하고
    // 만약 moveCount가 실제 칸 크기 이상이 되면 격자 위치를 변경한다. 
    float moveDistance = ARROW_SPEED * CELL_SIZE / FPS;
    switch (direction) {
    case Up:
        actualPosition.y -= moveDistance;
        break;
    case Down:
        actualPosition.y += moveDistance;
        break;
    case Left:
        actualPosition.x -= moveDistance;
        break;
    case Right:
        actualPosition.x += moveDistance;
        break;
    }

    moveCount += moveDistance;
    if (moveCount >= CELL_SIZE) {
        moveCount -= CELL_SIZE;
        lifeCount--;
        switch (direction) {
        case Up:
            position.second--;
            break;
        case Down:
            position.second++;
            break;
        case Left:
            position.first--;
            break;
        case Right:
            position.first++;
            break;
        }
    }

    // 주사기가 좀비에게 닿았을 때 좀비를 공격하고 소멸
    ofRectangle syringeRect(actualPosition.x, actualPosition.y, CELL_SIZE, CELL_SIZE);
    for (auto& z : gameManager.currRoom->zombies) {
        ofRectangle zombieRect(z->actualPosition.x, z->actualPosition.y, CELL_SIZE, CELL_SIZE);
        if (syringeRect.intersects(zombieRect)) {
            z->found = true;
            z->health -= damage;
            z->drawHealth();
            z->stun(SYRINGE_STUN_FRAME);
            if (z->health <= 0) {
                z->health = 0;
                z->isAlive = false;
            }
            isVisible = false;
        }
    }
}

// 주사기 그리기
void Syringe::draw() {
    if (isVisible) {
        ofVec2f drawPosition;
        switch (direction) {
        case Up:
        case Down:
            ofSetColor(0, 255, 255);
            drawPosition = actualPosition + ofVec2f(CELL_SIZE / 2 - DIRECTION_LINE_WIDTH, 0);
            ofDrawRectangle(drawPosition, DIRECTION_LINE_WIDTH * 2, CELL_SIZE);
            ofSetColor(0);
            ofDrawRectangle(drawPosition, DIRECTION_LINE_WIDTH * 2, SYRINGE_W);
            ofDrawRectangle(drawPosition + ofVec2f(0, CELL_SIZE - SYRINGE_W), DIRECTION_LINE_WIDTH * 2, SYRINGE_W);
            ofDrawRectangle(drawPosition, SYRINGE_W, CELL_SIZE);
            ofDrawRectangle(drawPosition + ofVec2f(DIRECTION_LINE_WIDTH * 2 - SYRINGE_W, 0), SYRINGE_W, CELL_SIZE);
            break;
        case Left:
        case Right:
            ofSetColor(0, 255, 255);
            drawPosition = actualPosition + ofVec2f(0, CELL_SIZE / 2 - DIRECTION_LINE_WIDTH);
            ofDrawRectangle(drawPosition, CELL_SIZE, DIRECTION_LINE_WIDTH * 2);
            ofSetColor(0);
            ofDrawRectangle(drawPosition, SYRINGE_W, DIRECTION_LINE_WIDTH * 2);
            ofDrawRectangle(drawPosition + ofVec2f(CELL_SIZE - SYRINGE_W, 0), SYRINGE_W, DIRECTION_LINE_WIDTH * 2);
            ofDrawRectangle(drawPosition, CELL_SIZE, SYRINGE_W);
            ofDrawRectangle(drawPosition + ofVec2f(0, DIRECTION_LINE_WIDTH * 2 - SYRINGE_W), CELL_SIZE, SYRINGE_W);
            break;
        }
        ofSetColor(255);
    }
}