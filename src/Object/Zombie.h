#pragma once

#include "../Main/Setting.h"
#include "ofMain.h"
#include "../Main/UI.h"
#include "ZombieType.h"
#include <vector>
#include <utility>

// 순환 종속성 방지를 위한 전방 선언
class Player;
class Room;

// 좀비 : 플레이어를 쫓아다니며 공격함
class Zombie {
public:
    bool isAlive;                                                                                               // 생존 여부
    bool isItemDropped;                                                                                         // 아이템 드랍 여부
    
    ZombieType zombieType;
    string name;
    bool found;                                                                                                 // 플레이어 발견 여부, 여기서는 구현하지 않음
    float detectRange;
    int maxHealth;                                                                                              // 좀비 최대 체력
    int health;                                                                                                 // 좀비 현재 체력
    
    bool canReborn;
    int rebornFrameCounter;

    int damage;                                                                                                 // 좀비 공격 피해량
    float speed;                                                                                                  // 좀비 이동 속도
    Room* room;                                                                                                 // 좀비가 있는 방

    int healthDrawCount;                                                                                        // 체력 표시 시간

    pair<int, int> position;                                                                                    // 좀비 격자 위치
    pair<int, int> indirectPosition;                                                                           // 좀비의 이동 방향 우선순위
    ofVec2f actualPosition;                                                                                     // 좀비 그리기 위치

    int stunDuration;                                                                                           // 좀비 스턴 시간
    int stunCounter;
    bool isStunned;                                                                                             // 좀비 스턴 여부

    bool canFreeMove;
    float freeMoveSpeed;
    bool isFreeMoving;
    pair<int, int> freeMoveTarget;

    int imageState;
    int imagePosCounter;
    const int imageChangePeriod = ZOMBIE_IMAGE_CHANGE_PERIOD;
    string imageName;
    ofImage currentImage;
    int characterHeight;

    Direction direction;

    Zombie(int x, int y, Room* room, ZombieType type);                                                    // 생성자
    virtual ~Zombie();                                                                                          // 소멸자

    void update(const pair<int, int>& targetPosition, const vector<vector<int>>& obstacles, Player& player);    // 업데이트
    void draw();                                                                                                // 그리기
    
    bool isValidMove(int x, int y, const vector<vector<int>>& obstacles);                                       // 해당 좌표로 이동이 가능한가
    pair<int, int> nextMove(const pair<int, int>& targetPosition, const vector<vector<int>>& obstacles);        // 다음 이동 좌표 계산
    void moveRandomly(const vector <vector <int>>& obstacles);

    void stun(int duration);                                                                                    // 좀비에게 스턴을 가함
    void drawHealth();                                                                                          // 좀비의 체력바 그리기
    void reborn();
    void drawTombstone();
    
    void changeImageName(bool posFlag, bool dirFlag);
    void loadImage();
};