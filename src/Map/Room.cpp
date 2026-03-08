#include "Room.h"
#include "../Main/GameManager.h"
#include <memory>

// 생성자
Room::Room(int row, int col) : 
    row(row),
    col(col),
    width(ROOM_SIZE),
    height(ROOM_SIZE),
    isAvailable(false),
    isIncluded(false),
    isClear(false),
    healthIncreased(false),
    isNear(false),
    isStart(false),
    isBoss(false),
    isPortal(false),
    isTreasure(false),
    isShop(false),
    neighbor(vector <bool> (DIRECTION_COUNT, false)),
    roomState(vector <vector <int>> (height, vector <int> (width, Empty))),
    zombieCnt(randInt(MIN_ZOMBIE_CNT, MAX_ZOMBIE_CNT)) {

    zombies.reserve(zombieCnt);

}

// 소멸자
Room::~Room() {

}

// 원형 장애물 설정
void Room::setCircle(vector <pair <int, int>>& circle, int radius) {
    for (int i = -radius; i <= radius; ++i) {
        int abs_i = abs(i);
        int remaining = radius - abs_i;

        circle.push_back({ width / 2 + i, height / 2 + remaining });
        if (remaining != 0) {
            circle.push_back({ width / 2 + i, height / 2 - remaining });
        }
    }
}

// 벽 설정
void Room::setWalls() {
    int randomRoomType = randInt(NormalRoomType + 1, MAP_TYPE_COUNT - 1);
    if (isPortal || isBoss) randomRoomType = 0;
    else if (isTreasure) randomRoomType = 0;
    else if (isShop) randomRoomType = 0;
    else if (isStart) randomRoomType = StartRoomType;
    for (int i = 0; i < ROOM_SIZE; i++) {
        roomState[i][0] = Wall;
        roomState[0][i] = Wall;
        roomState[i][ROOM_SIZE - 1] = Wall;
        roomState[ROOM_SIZE - 1][i] = Wall;
    }

    // 방의 타입에 따라 방의 장애물을 랜덤으로 설정
    switch (randomRoomType) {
    case Empty :    // 빈 방
        break;
    case StartRoomType :
        for (int i = 1; i < ROOM_SIZE / 2 - 2; i++) {
            roomState[ROOM_SIZE / 2 - 1][i] = Wall;
            roomState[ROOM_SIZE / 2 + 1][i] = Wall;
            roomState[i][ROOM_SIZE / 2 - 1] = Wall;
            roomState[i][ROOM_SIZE / 2 + 1] = Wall;
            roomState[ROOM_SIZE / 2 - 1][ROOM_SIZE - 1 - i] = Wall;
            roomState[ROOM_SIZE / 2 + 1][ROOM_SIZE - 1 - i] = Wall;
            roomState[ROOM_SIZE - 1 - i][ROOM_SIZE / 2 - 1] = Wall;
            roomState[ROOM_SIZE - 1 - i][ROOM_SIZE / 2 + 1] = Wall;
        }
        break;
    case OneInOne : // 한 쪽만 열린 원이 3개 있는 방
        {
            vector <vector <pair <int, int>>> circles;
            circles.resize(3);

            setCircle(circles[0], 2);
            setCircle(circles[1], 5);
            setCircle(circles[2], 9);

            for (int i = 0; i < 3; i++) {
                if (!circles.at(i).empty()) {
                    int removeIdx = rand() % circles.at(i).size();
                    circles.at(i).at(removeIdx) = circles.at(i).back();
                    circles.at(i).pop_back();
                }
            }

            for (auto circle : circles) {
                for (auto dot : circle) {
                    roomState[dot.first][dot.second] = Wall;
                }
            }
        }
        break;
    case Tetris :   // 테트리스 장애물들이 있는 방
        {
            vector <vector <pair <int, int>>> mino;
            int x_offset = randInt(2, 6);
            int y_offset = randInt(2, 6);
            mino.resize(4);
            for (int i = 0; i < 4; i++) {
                mino[i].resize(4);
                for (int j = 0; j < 4; j++) {
                    int posX = 5 * i + x_offset;
                    int posY = 5 * j + y_offset;
                    switch (randInt(6)) {
                    case 0:
                        switch (randInt(3)) {
                        case 0:
                            f(posX, posY, 1, 0);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        case 1:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 3, 1);
                            break;
                        case 2:
                            f(posX, posY, 2, 0);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 2, 3);
                            break;
                        case 3:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 3, 2);
                            break;
                        }
                        break;
                    case 1:
                        f(posX, posY, 1, 1);
                        f(posX, posY, 1, 2);
                        f(posX, posY, 2, 1);
                        f(posX, posY, 2, 2);
                        break;
                    case 2:
                        switch (randInt(3)) {
                        case 0:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 2);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 2);
                            break;
                        case 3:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 3:
                        switch (randInt(3)) {
                        case 0:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 3);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 3:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 4:
                        switch (randInt(3)) {
                        case 0:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 1);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 2, 3);
                            break;
                        case 3:
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 5:
                        switch (randInt(3)) {
                        case 0:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 1);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 2, 3);
                            break;
                        case 2:
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 2);
                            break;
                        case 3:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 6:
                        switch (randInt(3)) {
                        case 0:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 2, 2);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 3);
                            break;
                        case 3:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            break;
                        }
                        break;
                    }
                }
            }
            break;
        } 
        break;
    case Gaal :     // 갈! 이 그려져 있는 방
        {
            f(4, 8);
            f(4, 7);
            f(5, 7);
            f(6, 7);
            f(6, 6);
            f(7, 6);
            f(8, 6);
            f(9, 6);
            f(10, 6);
            f(10, 5);
            f(11, 5);
            f(12, 5);
            f(13, 5);
            f(13, 6);
            f(12, 6);
            f(12, 7);
            f(12, 8);
            f(11, 8);
            f(11, 9);
            f(11, 10);
            f(10, 10);
            f(10, 11);
            f(9, 11);
            f(9, 12);
            f(8, 12);
            f(8, 13);
            f(7, 13);
            f(7, 14);
            f(6, 14);

            for (int i = 2; i <= 12; i++) {
                f(15, i);
            }
            f(16, 8);
            f(17, 7);

            f(8, 16);
            f(9, 16);
            f(10, 16);
            f(10, 15);
            f(11, 15);
            f(12, 15);
            f(13, 14);
            f(14, 14);
            f(15, 14);
            f(15, 15);
            f(15, 16);
            f(15, 17);
            f(14, 17);
            f(13, 18);
            f(12, 18);
            f(11, 18);
            f(10, 18);
            f(9, 19);
            f(9, 20);
            f(9, 21);
            f(9, 22);
            f(10, 22);
            f(11, 22);
            f(12, 21);
            f(13, 21);
            f(14, 20);
            f(15, 20);
            f(15, 19);

            for (int i = 2; i <= 17; i++) {
                f(20, i);
            }
            f(20, 20);
        }
        break;
    case X :        // X 모양이 그려져 있는 방
        {
            for (int i = 4; i < ROOM_SIZE - 4; i++) {
                f(i, i);
                f(i, ROOM_SIZE - 1 - i);
            }
        }
        break;
    case ZigZag :   // 지그재그로 빽빽한 장애물들이 있는 방
        {
            for (int i = 2; i < ROOM_SIZE; i += 4) {
                if (i == ROOM_SIZE / 2) continue;
                for (int j = 1; j <= ROOM_SIZE / 3; j++) {
                    f(i, j);
                    f(i, ROOM_SIZE - 1 - j);
                }
            }
            for (int i = 4; i < ROOM_SIZE; i += 4) {
                if (i == ROOM_SIZE / 2) continue;
                for (int j = ROOM_SIZE / 3; j <= ROOM_SIZE / 2; j++) {
                    f(i, j);
                    f(i, ROOM_SIZE - 1 - j);
                }
            }
        }
        break;
    default :
        break;
    }
}

// 방의 좀비가 모두 처치되었는지 확인
bool Room::allZombiesDefeated() const {
    for (const auto& zombie : zombies) {
        if (zombie->isAlive || (zombie->zombieType == Giant_Zombie && zombie->rebornFrameCounter > 0)) {
            return false;
        }
    }
    return true;
}

// 방의 문을 닫음
void Room::closeDoors() {
    for (int i = 0; i < DIRECTION_COUNT; ++i) {
        if (neighbor[i]) {
            roomState[doors[i].first][doors[i].second] = Wall; // 벽으로 설정
        }
    }
}

// 방의 문을 엶
void Room::openDoors(GameManager& gameManager) {
    for (int i = 0; i < DIRECTION_COUNT; ++i) {
        if (neighbor[i]) {
            roomState[doors[i].first][doors[i].second] = Empty;
        }
    }
    isClear = true;

    if (!healthIncreased) {
        Medic* medic = dynamic_cast<Medic*>(gameManager.player.get());
        if (medic) {
            medic->health = min(medic->health + 50, medic->maxHealth);
            healthIncreased = true;
        }
    }
}

// 방의 문 위치 설정
void Room::setDoors() {
    doors.resize(4);
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        doors[i].first = ROOM_SIZE / 2 + ROOM_SIZE / 2 * dc[i];
        doors[i].second = ROOM_SIZE / 2 + ROOM_SIZE / 2 * dr[i];
        if (neighbor[i]) {
            if (isPortal || isStart) {
                roomState[doors[i].first][doors[i].second] = Empty; // 시작 방과 포탈 방은 문을 열어둠
            }
            else {
                roomState[doors[i].first][doors[i].second] = Wall; // 나머지 방은 문을 닫아둠
            }
        }
    }
}

// 방의 좀비 생성 및 위치 설정
void Room::setZombies() {
    if (isPortal || isStart || isTreasure || isShop) {
        zombieCnt = 0;
        zombies.reserve(zombieCnt);
        return;
    }
    if (isBoss) {
        zombieCnt = randInt(MIN_ZOMBIE_CNT * BOSS_ZOMBIE_MUL, MAX_ZOMBIE_CNT * BOSS_ZOMBIE_MUL);
    }
    zombies.reserve(zombieCnt);
    for (int i = 0; i < zombieCnt; i++) {
        int p, q;
        do {
            p = randInt(4, ROOM_SIZE - 5);
            q = randInt(4, ROOM_SIZE - 5);
        } while (roomState[p][q] != 0);
        if (i < 0.5 * zombieCnt) {
            zombies.push_back(make_unique <Zombie>(p, q, this, Manic_Zombie));
        }
        else if(i < 0.8 * zombieCnt) {
            zombies.push_back(make_unique <Zombie>(p, q, this, Giant_Zombie));
        }
        else {
            zombies.push_back(make_unique <Zombie>(p, q, this, Blind_Zombie));
        }
    }
}

// 방의 처치된 좀비 제거
void Room::deleteZombie() {
    for (auto it = zombies.begin(); it != zombies.end(); ) {
        if (!(*it)->isAlive && (*it)->rebornFrameCounter == 0) {
            it = zombies.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 방 그리기
void Room::draw(UI& ui) {
    // 방의 각 칸을 초록색으로 그림
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (roomState[i][j] == 1) {
                ofSetColor(ofColor::red);
            }
            else {
                if (isPortal && i == width / 2 && j == height / 2) {
                    ofSetColor(ofColor::purple);
                }
                else {
                    ofSetColor(ofColor::green);
                }
            }
            ofDrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
    }

    // 방의 각 칸의 테두리를 검은색으로 그림
    ofSetColor(ofColor::black);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            ofDrawRectangle(i * CELL_SIZE, j * CELL_SIZE, GRID_LINE_WIDTH, CELL_SIZE);
            ofDrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, GRID_LINE_WIDTH);
            ofDrawRectangle((i + 1) * CELL_SIZE - GRID_LINE_WIDTH, j * CELL_SIZE, GRID_LINE_WIDTH, CELL_SIZE);
            ofDrawRectangle(i * CELL_SIZE, (j + 1) * CELL_SIZE - GRID_LINE_WIDTH, CELL_SIZE, GRID_LINE_WIDTH);
        }
    }

    // 좀비들과 아이템들을 그림
    for (auto& z : zombies) {
        z->draw();
    }
    drawItems();
}

// 방의 아이템들 그리기
void Room::drawItems() {
    ofSetColor(255, 255, 255);
    for (const auto& item : droppedItems) {
        if (item.first) {
            auto& pos = item.second;
            string itemName = item.first->name;

            int x = pos.first * CELL_SIZE + CELL_SIZE / 2;
            int y = pos.second * CELL_SIZE + CELL_SIZE / 2;

            int originalWidth = item.first->image.getWidth();
            int originalHeight = item.first->image.getHeight();
            float aspectRatio = static_cast<float>(originalWidth) / static_cast<float>(originalHeight);

            int drawWidth, drawHeight;
            if (originalWidth > originalHeight) {
                drawWidth = CELL_SIZE;
                drawHeight = static_cast<int>(CELL_SIZE / aspectRatio);
            }
            else {
                drawHeight = CELL_SIZE;
                drawWidth = static_cast<int>(CELL_SIZE * aspectRatio);
            }

            item.first->image.draw(x - drawWidth / 2, y - drawHeight / 2, drawWidth, drawHeight);
        }
    }
}

// 위치에 장애물 설치
void Room::f(int i, int j) {
    roomState[i][j] = Wall;
}

// 위치 기준 상대 위치에 장애물 설치
void Room::f(int posX, int posY, int i, int j) {
    roomState[posX + i][posY + j] = Wall;
}

// 위치에 장애물 설치
void Room::f(int i, int j, int code) {
    roomState[i][j] = code;
}

// 위치 기준 상대 위치에 장애물 설치
void Room::f(int posX, int posY, int i, int j, int code) {
    roomState[posX + i][posY + j] = code;
}

void Room::drawShopItems() {
    for (size_t i = 0; i < shopItems.size(); ++i) {
        auto [x, y] = shopItemPositions[i];
        float imgWidth = shopItems[i]->image.getWidth();
        float imgHeight = shopItems[i]->image.getHeight();
        float scale = CELL_SIZE / max(imgWidth, imgHeight);

        float drawWidth = imgWidth * scale;
        float drawHeight = imgHeight * scale;

        float drawX = (x * CELL_SIZE) + (CELL_SIZE - drawWidth) / 2;
        float drawY = (y * CELL_SIZE) + (CELL_SIZE - drawHeight) / 2;

        shopItems[i]->image.draw(drawX, drawY, drawWidth, drawHeight);
    }
}
