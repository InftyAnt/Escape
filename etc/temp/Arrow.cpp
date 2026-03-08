#include "Arrow.h"
#include "Camera.h"

//--------------------------------------------------------------
Arrow::Arrow(Direction dir, std::pair <int, int> pos, int dmg) {
	isVisible = true;
	direction = dir;
	position = pos;
	speed = ARROW_SPEED;
	damage = dmg;
	lifeCount = 8;
	moveCount = 0;
	actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

//--------------------------------------------------------------
Arrow::~Arrow() {

}

//--------------------------------------------------------------
void Arrow::move() {
	if (moveCount == 0) {
		isVisible = false;
		return;
	}

    float moveDistance = ARROW_SPEED / FPS * CELL_SIZE;
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
}

void Arrow::draw(const Camera& camera) const {
    if (isVisible) {
        ofVec2f drawPosition = actualPosition - ofVec2f(camera.x * CELL_SIZE, camera.y * CELL_SIZE);
        ofSetColor(255, 0, 0);
        ofDrawRectangle(drawPosition, CELL_SIZE, CELL_SIZE);
    }
}