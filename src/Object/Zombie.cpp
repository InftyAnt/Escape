#include "Zombie.h"
#include "../Map/Room.h"
#include "Player.h"
#include <queue>
#include <chrono>
#include <cmath>

using namespace std::chrono;

// 생성자
Zombie::Zombie(int x, int y, Room* room, ZombieType type) :
    position(make_pair(x, y)),
    room(room),
    isAlive(true),
    isItemDropped(false),
    found(false),
    rebornFrameCounter(0),
    isStunned(false),
    stunDuration(0),
    healthDrawCount(0),
    zombieType(type),
    isFreeMoving(false),
    freeMoveTarget(make_pair(-1, -1)),
    imageState(0),
    imagePosCounter(0),
    direction(Down),
    characterHeight(0) {

    switch (zombieType) {
    case Manic_Zombie :
        maxHealth = 40;
        damage = 15;
        speed = A_LITTLE_SLOW_ZOMBIE_SPEED;
        detectRange = 5.0;
        canFreeMove = false;
        freeMoveSpeed = 0.0f;
        canReborn = false;
        imageName = "Giant_Zombie_Down_2.png";
        characterHeight = CELL_SIZE;
        break;
    case Giant_Zombie :
        maxHealth = 50;
        damage = 25;
        speed = NORMAL_ZOMBIE_SPEED;
        detectRange = 7.25;
        canFreeMove = true;
        freeMoveSpeed = NORMAL_ZOMBIE_SPEED;
        canReborn = true;
        imageName = "Giant_Zombie_Down_2.png";
        characterHeight = 125;
        break;
    case Blind_Zombie :
        maxHealth = 70;
        damage = 30;
        speed = FAST_ZOMBIE_SPEED;
        detectRange = 3.0;
        canFreeMove = true;
        freeMoveSpeed = VERY_SLOW_ZOMBIE_SPEED;
        canReborn = false;
        imageName = "Giant_Zombie_Down_2.png";
        characterHeight = CELL_SIZE;
        break;
    default :
        maxHealth = 10;
        damage = 1;
        speed = A_LITTLE_SLOW_ZOMBIE_SPEED;
        detectRange = 1.0;
        canFreeMove = false;
        freeMoveSpeed = 0.0f;
        canReborn = false;
        break;
    }

    health = maxHealth;
    actualPosition.set(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2);
    loadImage();
    
}

// 소멸자
Zombie::~Zombie() {

}

// 업데이트
void Zombie::update(const pair<int, int>& targetPosition, const vector<vector<int>>& obstacles, Player& player) {
    if (!isAlive) {
        if (canReborn && rebornFrameCounter > 0) {
            rebornFrameCounter--;
            if (rebornFrameCounter == 0) {
                reborn();
                loadImage(); // 부활 후 이미지 업데이트
            }
        }
        return;
    }


    // 이미지의 변경 여부 관리
    bool imagePosChange = false;
    bool imageDirChange = false;

    // 스턴 상태가 아니라면 -> 주기적으로 갱신
    if (!isStunned) {
        imagePosCounter++;
        if (imagePosCounter >= imageChangePeriod) {
            imagePosCounter = 0;
            imagePosChange = true;  // 주기적으로 좀비 포즈 변경
        }
    }
    // 스턴 상태인데 파닥파닥이 끝날 때가 된다면
    // 8의 의미 : 좀비는 스턴이 걸리더라도 포즈를 2주기 변경한다, 2주기 = 8회 포즈 변경
    else if (stunDuration == 8 * imageChangePeriod) {
        imagePosCounter = 0;
        imageState = 0;
        imagePosChange = true; // 스턴 상태일 때 이미지 변경 필요 플래그 설정
    }

    // 스턴 시간이 있다면 시간 소모
    if (isStunned) {
        stunCounter++;
        // 만약 스턴 시간이 끝나면 스턴 해제
        if (stunDuration <= stunCounter) {
            isStunned = false;
            stunCounter = 0;
            imagePosChange = true; // 스턴 해제 후 이미지 변경 필요 플래그 설정
        }
    }

    // 플레이어를 발견하기 전 움직이는 좀비 매커니즘
    if (isFreeMoving) {
        ofVec2f targetPosVec(freeMoveTarget.first * CELL_SIZE + CELL_SIZE / 2, freeMoveTarget.second * CELL_SIZE + CELL_SIZE / 2);
        ofVec2f directionVec = targetPosVec - actualPosition;
        directionVec.normalize();
        actualPosition += directionVec * freeMoveSpeed;

        // 위치 보정
        if ((targetPosVec - actualPosition).length() < freeMoveSpeed) {
            actualPosition = targetPosVec;
            position = freeMoveTarget;
            isFreeMoving = false;
        }
        changeImageName(imagePosChange, imageDirChange);
        return;
    }

    // 플레이어를 발견하지 않았을 때
    if (!found) {
        // 플레이어와의 거리 계산
        float dx = position.first - player.position.first;
        float dy = position.second - player.position.second;
        float distance = sqrt(dx * dx + dy * dy);

        // 플레이어가 감지 범위 안에 들어오면 발견
        if (distance <= detectRange) {
            found = true;
        }
        // 들어오지 않으면 자유 이동
        else if (canFreeMove) {

            if (!isFreeMoving) {
                moveRandomly(obstacles);
                imageDirChange = true;
            }

            if (isFreeMoving) {
                ofVec2f targetPosVec(freeMoveTarget.first * CELL_SIZE + CELL_SIZE / 2, freeMoveTarget.second * CELL_SIZE + CELL_SIZE / 2);
                ofVec2f directionVec = targetPosVec - actualPosition;
                directionVec.normalize();
                actualPosition += directionVec * freeMoveSpeed;

                if ((targetPosVec - actualPosition).length() < freeMoveSpeed) {
                    actualPosition = targetPosVec;
                    position = freeMoveTarget;
                    isFreeMoving = false;
                }
                changeImageName(imagePosChange, imageDirChange);
                return;
            }
        }
    }

    // 좀비가 플레이어의 존재를 발견
    if (found) {
        // 좀비 위치가 칸과 완전히 맞지 않으면 (칸 사이를 이동 중이면)
        if (actualPosition != ofVec2f(position.first * CELL_SIZE + CELL_SIZE / 2, position.second * CELL_SIZE + CELL_SIZE / 2)) {
            ofVec2f directionVec = ofVec2f(position.first * CELL_SIZE + CELL_SIZE / 2, position.second * CELL_SIZE + CELL_SIZE / 2) - actualPosition;
            directionVec.normalize();
            actualPosition += directionVec * speed;

            if ((ofVec2f(position.first * CELL_SIZE + CELL_SIZE / 2, position.second * CELL_SIZE + CELL_SIZE / 2) - actualPosition).length() < speed) {
                actualPosition.set(position.first * CELL_SIZE + CELL_SIZE / 2, position.second * CELL_SIZE + CELL_SIZE / 2);
            }
        }
        // 좀비가 칸에 완전이 들어왔는데 스턴 상태라면
        else if (!isStunned) {
            pair<int, int> nextPos = nextMove(targetPosition, obstacles);
            if (nextPos != position && isValidMove(nextPos.first, nextPos.second, obstacles)) {
                if (nextPos.first > position.first) direction = Right;
                else if (nextPos.first < position.first) direction = Left;
                else if (nextPos.second > position.second) direction = Down;
                else if (nextPos.second < position.second) direction = Up;

                position = nextPos;
                imageDirChange = true; // 방향이 바뀌었을 때 이미지 변경 플래그 설정
            }
        }
    }

    if (position == player.position && !player.isInvincible) {
        player.health -= damage;
        player.isInvincible = true;
        player.drawHealth();
    }
    if (player.health < 0) player.health = 0;

    // 이미지 변경을 위한 updateImage 호출
    changeImageName(imagePosChange, imageDirChange);
}


// 그리기
void Zombie::draw() {
    // 부활 중의 메커니즘
    if (!isAlive && canReborn) {
        drawTombstone();
        isStunned = false;
        stunCounter = 0;
        return;
    }

    if (!isAlive) return;

    if (!currentImage.isAllocated()) { // 이미지가 로드되지 않았을 때
        string ztype;
        switch (zombieType) {
        case Manic_Zombie:
            ztype = "Manic";
            break;
        case Giant_Zombie:
            ztype = "Giant";
            break;
        case Blind_Zombie:
            ztype = "Blind";
            break;
        }
            
        ofLogWarning("Zombie") << "Attempted to draw an unallocated texture: " << ztype;
    }

    int imageWidth = (CELL_SIZE * currentImage.getWidth()) / 200;
    int imageHeight = (CELL_SIZE * currentImage.getHeight()) / 200;
    ofSetColor(ofColor::blue);
    switch (zombieType) {
    case Manic_Zombie:
        ofDrawCircle(actualPosition.x, actualPosition.y, CELL_SIZE / 2);
        break;
    case Giant_Zombie:
        ofDrawRectangle(actualPosition.x - CELL_SIZE / 2, actualPosition.y - CELL_SIZE / 2, CELL_SIZE, CELL_SIZE);
        ofSetColor(ofColor::white);
        currentImage.draw(actualPosition.x - imageWidth / 2, actualPosition.y - CELL_SIZE - imageHeight / 2, imageWidth, imageHeight);
        break;
    case Blind_Zombie:
        ofDrawTriangle(actualPosition.x - CELL_SIZE / 2, actualPosition.y + CELL_SIZE / 2,
            actualPosition.x + CELL_SIZE / 2, actualPosition.y + CELL_SIZE / 2,
            actualPosition.x, actualPosition.y - CELL_SIZE / 2);
        break;
    default:
        break;
    }

    if (healthDrawCount > 0) {
        ofSetColor(127);
        ofDrawRectangle(actualPosition.x - CELL_SIZE / 2, actualPosition.y - characterHeight + ENTITY_HEALTHBAR_Y,
            CELL_SIZE, ENTITY_HEALTHBAR_W + ENTITY_HEALTHBAR_STROKE_W * 2);
        ofSetColor(ofColor::red);
        ofDrawRectangle(actualPosition.x - CELL_SIZE / 2 + ENTITY_HEALTHBAR_STROKE_W,
            actualPosition.y - characterHeight + ENTITY_HEALTHBAR_Y + ENTITY_HEALTHBAR_STROKE_W,
            (CELL_SIZE - ENTITY_HEALTHBAR_STROKE_W * 2) * health / maxHealth, ENTITY_HEALTHBAR_W);
        ofSetColor(255);
        healthDrawCount--;
    }
}


// 해당 좌표로 이동이 가능한가
bool Zombie::isValidMove(int x, int y, const vector<vector<int>>& obstacles) {
    return x >= 0 && y >= 0 && x < obstacles.size() && y < obstacles[0].size() && obstacles[x][y] == 0;
}

// 유클리드 거리 계산 함수 추가
double euclideanDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// 우선순위 큐에서 사용할 비교 함수 추가
struct Node {
    int x, y;
    int cost, priority;
    bool operator<(const Node& other) const {
        return priority > other.priority;  // 우선순위가 낮을수록 높은 우선순위
    }
};

// nextMove 함수 수정 (A* 알고리즘 사용, 유클리드 거리 사용)
pair<int, int> Zombie::nextMove(const pair<int, int>& targetPosition, const vector<vector<int>>& obstacles) {

    int startX = position.first;
    int startY = position.second;
    int targetX = targetPosition.first;
    int targetY = targetPosition.second;

    if (startX == targetX && startY == targetY) {
        return make_pair(startX, startY);
    }

    int relativeX = startX - targetX;
    int relativeY = startY - targetY;

    Direction dir;

    if (relativeX + relativeY < 0) {
        if (relativeX - relativeY > 0) {
            dir = Down;
        }
        else {
            dir = Right;
        }
    }
    else {
        if (relativeX - relativeY > 0) {
            dir = Left;
        }
        else {
            dir = Up;
        }
    }

    priority_queue<Node> openSet;
    openSet.push({ startX, startY, 0, static_cast<int>(euclideanDistance(startX, startY, targetX, targetY)) });

    vector<vector<int>> cost(obstacles.size(), vector<int>(obstacles[0].size(), INT_MAX));
    cost[startX][startY] = 0;

    vector<vector<pair<int, int>>> cameFrom(obstacles.size(), vector<pair<int, int>>(obstacles[0].size(), make_pair(-1, -1)));

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == targetX && current.y == targetY) {
            break;
        }

        for (int d = 0; d < DIRECTION_COUNT; d++) {
            int newX = current.x + dc[d];
            int newY = current.y + dr[d];

            if (isValidMove(newX, newY, obstacles)) {
                int newCost = cost[current.x][current.y] + 1;
                if (newCost < cost[newX][newY]) {
                    cost[newX][newY] = newCost;
                    int priority = newCost + static_cast<int>(euclideanDistance(newX, newY, targetX, targetY));
                    openSet.push({ newX, newY, newCost, priority });
                    cameFrom[newX][newY] = make_pair(current.x, current.y);
                }
            }
        }
    }

    if (cost[targetX][targetY] == INT_MAX) {
        return position;
    }

    vector<int> candidate_dir;
    int currX = targetX, currY = targetY;
    while (cameFrom[currX][currY] != make_pair(startX, startY)) {
        pair<int, int> prev = cameFrom[currX][currY];
        currX = prev.first;
        currY = prev.second;
    }

    for (int d = 0; d < DIRECTION_COUNT; d++) {
        if (startX + dc[d] == currX && startY + dr[d] == currY) {
            candidate_dir.push_back(d);
        }
    }

    bool existanceFlag = false;
    int movingDir;
    for (int d : candidate_dir) {
        if (d == static_cast<int>(dir)) {
            existanceFlag = true;
            movingDir = d;
            break;
        }
    }

    if (!existanceFlag) {
        movingDir = candidate_dir[0];
    }

    return make_pair(startX + dc[movingDir], startY + dr[movingDir]);
}

void Zombie::moveRandomly(const vector<vector<int>>& obstacles) {
    if (!isFreeMoving) {
        static vector<pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
        vector<pair<int, int>> possibleMoves;

        for (auto [dx, dy] : directions) {
            int newX = position.first + dx;
            int newY = position.second + dy;
            if (isValidMove(newX, newY, obstacles)) {
                possibleMoves.push_back({ newX, newY });
            }
        }

        if (!possibleMoves.empty()) {
            auto [newX, newY] = possibleMoves[rand() % possibleMoves.size()];
            freeMoveTarget = { newX, newY };
            isFreeMoving = true;
            if (newX == position.first) {
                if (newY > position.second) direction = Down;
                else direction = Up;
            }
            else if (newX > position.first) direction = Right;
            else direction = Left;
        }
    }
}

// 좀비에게 스턴을 가함
void Zombie::stun(int duration) {
    stunDuration = duration;
    stunCounter = 0;
    isStunned = true;
}

// 좀비의 체력바 그리기
void Zombie::drawHealth() {
    healthDrawCount = ENTITY_HEALTH_DRAW_TIME;
}

void Zombie::reborn() {
    isAlive = true;
    health = maxHealth;
    canReborn = false;  // 부활 후 다시 부활 불가 설정
    rebornFrameCounter = 0;
}

void Zombie::drawTombstone() {
    int x = position.first * CELL_SIZE;
    int y = position.second * CELL_SIZE;
    int height = (CELL_SIZE * rebornFrameCounter) / ZOMBIE_REBORN_TIME;  // 초기값 180 기준
    ofSetColor(ofColor::gray);
    ofDrawRectangle(x, y + CELL_SIZE - height, CELL_SIZE, height);
}

string zombieTypeToString(ZombieType type) {
    switch (type) {
    case Manic_Zombie:
        break;
    case Giant_Zombie:
        break;
    case Blind_Zombie:
        break;
    default:
        break;
    }
    return "Giant_Zombie";
}

string directionToString(Direction dir) {
    switch (dir) {
    case Up:
        return "Up";
    case Down:
        return "Down";
    case Left:
        return "Left";
    case Right:
        return "Right";
    default:
        return "Unknown";
    }
}

void Zombie::changeImageName(bool posFlag, bool dirFlag) {
    if (!posFlag && !dirFlag) return;
    if (posFlag) {
        imageState++;
        if (imageState >= 4) imageState = 0;
    }
    int imageNumber = imageState % 2 ? imageState : 2;
    imageName = zombieTypeToString(zombieType) + "_" + directionToString(direction) + "_" + to_string(imageNumber) + ".png";
    loadImage();
}

void Zombie::loadImage() {
    if (!currentImage.loadImage(imageName)) {
        ofLogWarning("Zombie") << imageName << " failed to load.";
    }
}