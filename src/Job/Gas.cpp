#include "Gas.h"
#include "../Main/GameManager.h"

// 생성자
Gas::Gas(Direction dir, pair <int, int> pos, int dmg) {
	isVisible = true;
	isAttacked = false;
	direction = dir;
	position = pos;
	damage = dmg;
	animationCount = GAS_ANIMATION_TIME * FPS;
}

// 가스 이동 (공격)
void Gas::move(const GameManager& gameManager) {
    if (animationCount == 0) {
        isVisible = false;
        return;
    }
    animationCount--;

    if (isAttacked) {
        return;
    }

    // hitRange 계산 후 해당 좌표 내에 있는 모든 좀비들을 타격한 후 가스의 타격 여부 갱신
    hitRange = getAttackRange();
    for (const auto& pos : hitRange) {
        for (auto& z : gameManager.currRoom->zombies) {
            if (z->position == pos) {
                z->found = true;
                z->health -= damage;
                z->drawHealth();
                z->stun(GAS_STUN_FRAME);
                if (z->health <= 0) {
                    z->health = 0;
                    z->isAlive = false;
                }
            }
        }
    }
    isAttacked = true;
}

// 가스 그리기
void Gas::draw() {
    if (isVisible) {
        ofSetColor(ofColor::red, 127);
        for (const auto& pos : hitRange) {
            ofDrawRectangle(pos.first * CELL_SIZE, pos.second * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
        ofSetColor(255, 255);
    }

}

// hitRange 계산
vector<pair<int, int>> Gas::getAttackRange() {
    vector<pair<int, int>> range;
    int x = position.first;
    int y = position.second;

    // 방향에 따른 hitRange 계산
    switch (direction) {
    case Up:
        for (int i = -1; i <= 1; ++i) {
            for (int j = -3; j <= -1; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    case Down:
        for (int i = -1; i <= 1; ++i) {
            for (int j = 1; j <= 3; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    case Left:
        for (int i = -3; i <= -1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                range.push_back(make_pair(x + i, y + j));
            }
        }
        break;
    case Right:
        for (int i = 1; i <= 3; ++i) {
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