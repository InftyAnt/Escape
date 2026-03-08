#include "Chair.h"
#include "../Main/GameManager.h"

// 생성자
Chair::Chair(Direction dir, pair <int, int> pos, int dmg) {
	isVisible = true;
    isAttacked = false;
	direction = dir;
	position = pos;
	damage = dmg;
	animationCount = CHAIR_ANIMATION_TIME * FPS;
}

// 의자 이동 (공격)
void Chair::move(const GameManager& gameManager) {
    if (animationCount == 0) {
        isVisible = false;
        return;
    }
    animationCount--;
    if (isAttacked) return;

    // hitRange 계산 후 해당 좌표 내에 있는 모든 좀비들을 타격한 후 의자의 타격 여부 갱신
    hitRange = getAttackRange();
    for (const auto& pos : hitRange) {
        for (auto& z : gameManager.currRoom->zombies) {
            if (z->position == pos) {
                z->found = true;
                z->health -= damage;
                z->drawHealth();
                if (z->health <= 0) {
                    z->health = 0;
                    z->isAlive = false;
                }
            }
        }
    }
    isAttacked = true;
}

// 의자 그리기
void Chair::draw() {
    if (isVisible) {
        ofSetColor(ofColor::red, 127);
        for (const auto& pos : hitRange) {
            ofDrawRectangle(pos.first * CELL_SIZE, pos.second * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
        ofSetColor(255);
    }
    
}

// hitRange 계산
vector<pair<int, int>> Chair::getAttackRange() {
    vector<pair<int, int>> range;
    int x = position.first;
    int y = position.second;

    // 방향에 따른 hitRange 계산
    switch (direction) {
    case Up:
        for (int i = -1; i <= 1; ++i) {
            for (int j = -2; j <= -1; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    case Down:
        for (int i = -1; i <= 1; ++i) {
            for (int j = 1; j <= 2; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    case Left:
        for (int i = -2; i <= -1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    case Right:
        for (int i = 1; i <= 2; ++i) {
            for (int j = -1; j <= 1; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    default:
        break;
    }

    return range;
}