#include "Arrow.h"
#include "../Main/Camera.h"
#include "../Main/GameManager.h"

// 생성자
Arrow::Arrow(Direction dir, pair <int, int> pos, int dmg) {
	isVisible = true;
	direction = dir;
	position = pos;
	speed = ARROW_SPEED;
	damage = dmg;
	lifeCount = 7;
	moveCount = 0;
	actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

// 화살 이동
void Arrow::move(const GameManager& gameManager) {
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

    // 화살이 좀비에게 닿았을 때 좀비를 공격
    ofRectangle arrowRect(actualPosition.x, actualPosition.y, CELL_SIZE, CELL_SIZE);
    for (auto& z : gameManager.currRoom->zombies) {
        if (hitZombies.find(z.get()) != hitZombies.end()) {
            continue;
        }
        ofRectangle zombieRect(z->actualPosition.x, z->actualPosition.y, CELL_SIZE, CELL_SIZE);
        if (arrowRect.intersects(zombieRect)) {
            z->found = true;
            z->health -= damage;
            z->drawHealth();
            if (z->health <= 0) {
                z->health = 0;
                z->isAlive = false;
            }
            hitZombies.insert(z.get());
        }
    }
}

// 화살 그리기
void Arrow::draw() {
    if (isVisible) {
        ofVec2f drawPosition;
        ofSetColor(0);
        switch (direction) {
        case Up :
        case Down :
            drawPosition = actualPosition + ofVec2f(CELL_SIZE / 2 - DIRECTION_LINE_WIDTH, 0);
            ofDrawRectangle(drawPosition, DIRECTION_LINE_WIDTH * 2, CELL_SIZE);
            break;
        case Left :
        case Right :
            drawPosition = actualPosition + ofVec2f(0, CELL_SIZE / 2 - DIRECTION_LINE_WIDTH);
            ofDrawRectangle(drawPosition, CELL_SIZE, DIRECTION_LINE_WIDTH * 2);
            break;
        }
        ofSetColor(255);
    }
}