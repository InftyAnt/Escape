#include "GameManager.h"

// 생성자
GameManager::GameManager(Job job) :
    player(make_unique<Player>(Down, ARCHER)),
    camera(GAMEBOARD_WIDTH * CELL_SIZE, GAMEBOARD_HEIGHT * CELL_SIZE, ROOM_SIZE * CELL_SIZE, ROOM_SIZE * CELL_SIZE),
    ui(),
    isGameOver(false) {

    map = make_unique<Map>(MIN_ROOM_CNT, MAX_ROOM_CNT);
    currRoom = map->start;

    fbo.allocate(CELL_SIZE * ROOM_SIZE, CELL_SIZE * ROOM_SIZE);
    fbo.begin();
    ofClear(255, 255, 255, 255);
    fbo.end();
    font.load("verdana.ttf", 32);
    initIP();
    initMP();

    // 선택된 직업으로 설정
    switch (job) {
    case ARCHER :
        changePlayer <Archer>();
        break;
    case MEDIC :
        changePlayer <Medic>();
        break;
    case CHAIRMAN :
        changePlayer <Chairman>();
        break;
    case FIREMAN :
        changePlayer <Fireman>();
        break;
    }

    for (auto& shop : map->shop) {
        createShopItems(*shop);
    }

}

// 소멸자
GameManager::~GameManager() {
    fbo.destroy();
}

// 업데이트
void GameManager::update() {
    if (player->health == 0) isGameOver = true;
    if (isGameOver) return;

    // 직업 별 행동 업데이트
    Archer* archer = dynamic_cast<Archer*>(player.get());
    if (archer) {
        for (auto& arrow : archer->arrows) {
            arrow->move(*this);
        }
        archer->deleteArrow();
    }
    Medic* medic = dynamic_cast<Medic*>(player.get());
    if (medic) {
        for (auto& syringe : medic->syringes) {
            syringe->move(*this);
        }
        medic->deleteSyringe();
    }
    Chairman* chairman = dynamic_cast<Chairman*>(player.get());
    if (chairman) {
        for (auto& chair : chairman->chairs) {
            chair->move(*this);
        }
        chairman->deleteChair();
    }
    Fireman* fireman = dynamic_cast<Fireman*>(player.get());
    if (fireman) {
        for (auto& g : fireman->gas) {
            g->move(*this);
        }
        fireman->update(*currRoom);
    }
    else {
        player->update(*currRoom);
    }

    // 카메라 업데이트
    camera.update(*player);

    // 좀비 처치 시 아이템/미네랄 드랍 및 부활
    for (auto& z : currRoom->zombies) {
        z->update(player->position, currRoom->roomState, *player);
        if (!z->isAlive) {
            if (z->canReborn && z->rebornFrameCounter == 0) {
                z->rebornFrameCounter = ZOMBIE_REBORN_TIME;
            }
            else if (!z->canReborn && !z->isItemDropped) {
                z->isItemDropped = true;
                if (ofRandom(100) <= ITEM_DROP_P) {
                    auto item = getRandomItem();
                    if (item) {
                        currRoom->droppedItems.push_back({ move(item), z->position });
                    }
                }
                auto mineral = getRandomMineral();
                if (mineral) {
                    currRoom->droppedItems.push_back({ move(mineral), z->position });
                }
            }
        }
    }

    // 부활 타이머 관리
    for (auto& z : currRoom->zombies) {
        if (!z->isAlive && z->rebornFrameCounter > 0) {
            z->rebornFrameCounter--;
            if (z->rebornFrameCounter == 0) {
                z->reborn();
            }
        }
    }

    currRoom->deleteZombie();

    // 플레이어의 아이템 획득
    auto collectPosition = player->position;
    for (auto it = currRoom->droppedItems.begin(); it != currRoom->droppedItems.end();) {
        if (it->second == collectPosition) {
            auto itemPtr = it->first->clone();
            bool itemAdded = player->addItem(move(itemPtr));
            if (itemAdded) {
                it = currRoom->droppedItems.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    if (player->isEPressed) {
        interactWithShop();
    }

    // 플레이어가 포탈에 접근했을 때 새 맵으로 이동
    if (currRoom->isPortal && player->position == make_pair(currRoom->width / 2, currRoom->height / 2)) {
        createNewMap();
    }

    // 방 전환
    changeRoom();
}

// 그리기
void GameManager::draw() {

    // 게임 화면 설정 시작
    fbo.begin();

    // 현재 방 그리기
    currRoom->draw(ui);
    
    if (currRoom->isShop) {
        currRoom->drawShopItems();
    }

    if (currRoom && !currRoom->isPortal && !currRoom->isStart && currRoom->allZombiesDefeated()) {
        currRoom->openDoors(*this);
    }

    // 직업 별 공격 수단 그리기
    Archer* archer = dynamic_cast<Archer*>(player.get());
    if (archer) {
        for (auto& arrow : archer->arrows) {
            arrow->draw();
        }
    }
    Medic* medic = dynamic_cast<Medic*>(player.get());
    if (medic) {
        for (auto& syringe : medic->syringes) {
            syringe->draw();
        }
    }
    Chairman* chairman = dynamic_cast<Chairman*>(player.get());
    if (chairman) {
        for (auto& chair : chairman->chairs) {
            chair->draw();
        }
    }
    Fireman* fireman = dynamic_cast<Fireman*>(player.get());
    if (fireman) {
        for (auto& g : fireman->gas) {
            if (fireman->drawCool > 0) {
                g->draw();
            }
        }
    }

    // 플레이어 그리기
    player->draw(ui);

    // 게임 화면 설정 종료
    fbo.end();

    // 게임 화면 출력
    int windowWidth = ofGetWidth();
    int windowHeight = ofGetHeight();
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    fbo.getTexture().drawSubsection(
        (centerX - camera.width / 2),     // 화면 중앙에 맞추기 위한 X 좌표
        (centerY - camera.height / 2),    // 화면 중앙에 맞추기 위한 Y 좌표
        camera.width,                     // 화면에 그릴 너비
        camera.height,                    // 화면에 그릴 높이
        camera.x,                         // FBO에서 잘라낼 시작 X 좌표
        camera.y,                         // FBO에서 잘라낼 시작 Y 좌표
        camera.width,                     // 잘라낼 너비
        camera.height                     // 잘라낼 높이
    );

    // ui 그리기
    ui.drawMinimap(*this, *map);
    ui.drawHealthBar(*player);
    ui.drawCool(*player);
    ui.drawInventory(*player);
    ui.drawMineral(*player);
    ui.showRoomZombieState(*this);

    // 게임 오버 시 게임 오버 출력
    if (isGameOver) {
        drawGameOver();
    }
}

// 방 전환
void GameManager::changeRoom() {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        if (player->direction == i && currRoom->neighbor[i] && player->position == currRoom->doors[i]) {
            int newRow = currRoom->row + dr[i], newCol = currRoom->col + dc[i];
            currRoom = map->rooms[newRow][newCol].get();
            player->telePort(ROOM_SIZE / 2 + (ROOM_SIZE / 2 - 1) * dc[(i + 2) % 4], ROOM_SIZE / 2 + (ROOM_SIZE / 2 - 1) * dr[(i + 2) % 4]);
            player->updateTargetPosition();
            camera.update(*player);
        }
    }
}

// 게임 오버 그리기
void GameManager::drawGameOver() {
    ofSetColor(127, 191);
    ofDrawRectangle(ofGetWidth() / 4, ofGetHeight() / 4, ofGetWidth() / 2, ofGetHeight() / 2);
    ofSetColor(255);
    string text = "G A M E     O V E R !";
    font.drawString(text, ofGetWidth() / 2 - font.stringWidth(text) / 2, ofGetHeight() / 2 - font.stringHeight(text) / 2);
}

// 플레이어 직업 설정
template<typename T>
void GameManager::changePlayer() {
    auto oldPlayerState = player->position;
    auto oldPlayerHealth = player->health;

    player = make_unique<T>(player->direction);
    player->position = oldPlayerState;
    player->health = oldPlayerHealth;

    player->updateTargetPosition();
    camera.update(*player);
}

// 랜덤 아이템 선택
unique_ptr<Item> GameManager::getRandomItem() {
    int randVal = randInt(0, 99);
    int cumulativeProb = 0;
    for (const auto& itemPair : itemProbabilities) {
        cumulativeProb += itemPair.second;
        if (randVal <= cumulativeProb) {
            if (dynamic_cast<Bandage*>(itemPair.first.get())) {
                return make_unique<Bandage>();
            }
            else if (dynamic_cast<PainKiller*>(itemPair.first.get())) {
                return make_unique<PainKiller>();
            }
            else if (dynamic_cast<MedicalKit*>(itemPair.first.get())) {
                return make_unique<MedicalKit>();
            }
            else if (dynamic_cast<Multivitamin*>(itemPair.first.get())) {
                return make_unique<Multivitamin>();
            }
            else if (dynamic_cast<Protein*>(itemPair.first.get())) {
                return make_unique<Protein>();
            }
        }
    }
    return nullptr;
}

// 랜덤 미네랄 선택
unique_ptr<Item> GameManager::getRandomMineral() {
    int randVal = randInt(0, 99);
    int cumulativeProb = 0;
    for (const auto& itemPair : mineralProbabilities) {
        cumulativeProb += itemPair.second;
        if (randVal <= cumulativeProb) {
            if (dynamic_cast<SmallMineral*>(itemPair.first.get())) {
                return make_unique<SmallMineral>();
            }
            else if (dynamic_cast<MiddleMineral*>(itemPair.first.get())) {
                return make_unique<MiddleMineral>();
            }
            else if (dynamic_cast<BigMineral*>(itemPair.first.get())) {
                return make_unique<BigMineral>();
            }
        }
    }
    return nullptr;
}

// 아이템의 확률 정보 초기화
void GameManager::initIP() {
    itemProbabilities.push_back({ make_unique<Bandage>(), 30 });
    itemProbabilities.push_back({ make_unique<PainKiller>(), 20 });
    itemProbabilities.push_back({ make_unique<MedicalKit>(), 10 });
    itemProbabilities.push_back({ make_unique<Multivitamin>(), 5 });
    itemProbabilities.push_back({ make_unique<Protein>(), 35 });
}

// 미네랄의 확률 정보 초기화
void GameManager::initMP() {
    mineralProbabilities.push_back({ make_unique<SmallMineral>(), 60 });
    mineralProbabilities.push_back({ make_unique<MiddleMineral>(), 30 });
    mineralProbabilities.push_back({ make_unique<BigMineral>(), 10 });
}

// 새 맵 생성
void GameManager::createNewMap() {
    auto newMap = make_unique<Map>(MIN_ROOM_CNT, MAX_ROOM_CNT);
    currRoom = newMap->start;

    player->health = player->maxHealth;
    player->position = { ROOM_SIZE / 2, ROOM_SIZE / 2 };
    player->updateTargetPosition();

    camera.update(*player);

    map = move(newMap);
}

void GameManager::createShopItems(Room& shop) {
    vector<pair<int, int>> shopItemPositions = { {10, 12}, {13, 13}, {16, 12} };

    for (int i = 0; i < 3; ++i) {
        auto item = getRandomItem();  // 상점 아이템 결정 로직
        if (item) {
            shop.shopItems.push_back(move(item));
            // 각 아이템의 위치 설정
            shop.shopItemPositions.push_back(shopItemPositions[i]);
            // 아이템 위치를 벽으로 설정
            shop.roomState[shopItemPositions[i].first][shopItemPositions[i].second] = Wall;
        }
    }
}   

void GameManager::interactWithShop() {
    if (currRoom->isShop) {
        for (size_t i = 0; i < currRoom->shopItemPositions.size(); ++i) {
            auto [x, y] = currRoom->shopItemPositions[i];
            if (abs(player->position.first - x) <= 1 && abs(player->position.second - y) <= 1) {
                if (player->mineral >= currRoom->shopItems[i]->price) {  // 아이템의 가격 사용
                    player->mineral -= currRoom->shopItems[i]->price;
                    player->addItem(move(currRoom->shopItems[i]));
                    currRoom->shopItems.erase(currRoom->shopItems.begin() + i);
                    currRoom->shopItemPositions.erase(currRoom->shopItemPositions.begin() + i);
                    break;
                }
            }
        }
    }
}