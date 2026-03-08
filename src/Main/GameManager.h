#pragma once

#include "ofMain.h"
#include "../Object/Player.h"
#include "../Job/Archer.h"
#include "../Job/Medic.h"
#include "../Job/Chairman.h"
#include "../Job/Fireman.h"
#include "Camera.h"
#include "../Map/Map.h"
#include "UI.h"
#include "../Item/Item.h"
#include <vector>
#include <memory>
#include <utility>

// 게임의 진행을 관리하는 게임 매니저
class GameManager {
public:
    unique_ptr <Player> player;                                     // 게임 플레이어
    vector <pair <unique_ptr <Item>, int>> itemProbabilities;       // 아이템의 확률 정보
    vector <pair <unique_ptr <Item>, int>> mineralProbabilities;    // 아이템의 확률 정보
    Camera camera;                                                  // 카메라
    unique_ptr <Map> map;                                           // 맵
    Room* currRoom;                                                 // 현재 방
    UI ui;                                                          // 유저 인터페이스
    bool isGameOver;                                                // 게임 오버인가
    ofFbo fbo;                                                      // 프레임 버퍼 객체 : 여기에 그린 후 카메라를 통해 원하는 부분만 화면에 출력
    ofTrueTypeFont font;                                            // 폰트

    GameManager(Job job);                                           // 생성자
    ~GameManager();                                                 // 소멸자
    void update();                                                  // 업데이트
    void draw();                                                    // 그리기
    void changeRoom();                                              // 방 전환
    void drawGameOver();                                            // 게임 오버 그리기
    template<typename T>
    void changePlayer();                                            // 플레이어 직업 설정
    unique_ptr <Item> getRandomItem();                              // 랜덤 아이템 선택
    unique_ptr <Item> getRandomMineral();                           // 랜덤 미네랄 선택
    void initIP();                                                  // 아이템의 확률 정보 초기화
    void initMP();                                                  // 미네랄의 확률 정보 초기화

    void createNewMap();                                            // 새 맵 생성

    void createShopItems(Room& shop);
    void interactWithShop();

};
