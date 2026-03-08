#pragma once

#include "../Main/Setting.h"
#include "ofMain.h"
#include <memory>
#include <string>

#define MULTIVITAMIN_PACK_PERCENTAGE 50
#define MULTIVITAMIN_PACK_FRAME 60 * FPS

// 아이템의 속성, 현재는 CONSUMABLE 아이템만 구현되어 있음
typedef enum ItemType {
    CONSUMABLE,
    EQUIPPABLE,
    MINERAL
} ItemType;

// 순환 종속성 방지를 위한 전방 선언
class Player;

// 아이템, 플레이어는 아이템 획득 후 사용 가능하다
class Item {
public:
    ItemType itemType;                          // 아이템 속성
    string name;                                // 아이템 이름
    ofImage image;                              // 아이템 이미지
    string imagePath;                           // 아이템 이미지의 경로
    int price;

    Item(ItemType i, const string& n, int p);   // 생성자
    Item(const Item& other);                    // 복사 생성자
    virtual ~Item() = default;                  // 소멸자

    void loadImage();                           // 이미지 로드
    
    virtual void use(Player& player) = 0;       // 아이템 사용
    virtual unique_ptr<Item> clone() const = 0; // 아이템 복사
};

// 붕대
class Bandage : public Item {
public:
    Bandage();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

// 진통제
class PainKiller : public Item {
public:
    PainKiller();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

// 응급키트
class MedicalKit : public Item {
public:
    MedicalKit();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

// 종합비타민
class Multivitamin : public Item {
public:
    Multivitamin();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

// 프로틴
class Protein : public Item {
public:
    Protein();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

class SmallMineral : public Item {
public :
    SmallMineral();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

class MiddleMineral : public Item {
public:
    MiddleMineral();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};

class BigMineral : public Item {
public:
    BigMineral();
    void use(Player& player) override;
    unique_ptr<Item> clone() const override;
};