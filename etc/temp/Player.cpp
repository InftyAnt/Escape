#include "Player.h"

Player::Player(Direction dir, Job j) {
    isAlive = true;
    health = 10;
    job = j;
    direction = dir;
    speed = DEFAULT_PLAYER_SPEED;
    moveCount = 0;
    isMoving = false;
    activeKey = -1;
    position = { ROOM_SIZE / 2, ROOM_SIZE / 2 };
    actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
    targetPosition = actualPosition;
}

Player::~Player() {
}

void Player::updateTargetPosition() {
    targetPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

bool Player::canMoveTo(int x, int y, const Room& room) const {
    return x >= 0 && x < room.width && y >= 0 && y < room.height && !room.roomState[x][y];
}

void Player::move(const Room& room) {
    if (actualPosition != targetPosition) {
        ofVec2f directionVec = targetPosition - actualPosition;
        directionVec.normalize();
        actualPosition += directionVec * speed / FPS * CELL_SIZE;

        if ((targetPosition - actualPosition).length() < speed / FPS * CELL_SIZE) {
            actualPosition = targetPosition;
        }
    }
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
        }
    }
}

void Player::startMove(int key) {
    if (keyStack.empty() || key != keyStack.top()) {
        keyStack.push(key);
    }
    activeKey = key;
    isMoving = true;
}

void Player::finishMove() {
    isMoving = false;
    activeKey = -1;
}

void Player::keyPressed(int key) {
    startMove(key);
}

void Player::keyReleased(int key) {
    if (!keyStack.empty() && keyStack.top() == key) {
        keyStack.pop();
        if (!keyStack.empty()) {
            activeKey = keyStack.top();
        }
        else {
            activeKey = -1;
            finishMove();
            updateTargetPosition();
        }
    }
    else {
        std::stack<int> tempStack;
        while (!keyStack.empty() && keyStack.top() != key) {
            tempStack.push(keyStack.top());
            keyStack.pop();
        }
        if (!keyStack.empty()) {
            keyStack.pop();
        }
        while (!tempStack.empty()) {
            keyStack.push(tempStack.top());
            tempStack.pop();
        }
        if (keyStack.empty()) {
            activeKey = -1;
            finishMove();
            updateTargetPosition();
        }
    }
}

void Player::firstSkill() {
}

void Player::secondSkill() {
}

void Player::telePort(int r, int c) {
    position = { r, c };
    actualPosition = ofVec2f(position.first * CELL_SIZE, position.second * CELL_SIZE);
}

void Player::draw() {
    int DirectionLineLength = CELL_SIZE / 2 + DIRECTION_LINE_WIDTH;
    int Remnant = CELL_SIZE / 2 - DIRECTION_LINE_WIDTH;
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
