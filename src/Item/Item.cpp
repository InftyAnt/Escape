#include "Item.h"
#include "../Object/Player.h"

// 생성자
Item::Item(ItemType i, const string& n, int p) : itemType(i), name(n), price(p) {
    imagePath = name + ".png";
    loadImage();
}

// 복사 생성자
Item::Item(const Item& other) : itemType(other.itemType), name(other.name), image(other.image), imagePath(other.imagePath) {}

// 이미지 로드
void Item::loadImage() {
    if (!image.load(imagePath)) {
        ofLogError("Item") << "Failed to load image: " << imagePath;
    }
}

// 붕대 생성자
Bandage::Bandage() : Item(CONSUMABLE, "Bandage", 100) {}

// 붕대 사용 : 체력 25 회복
void Bandage::use(Player& player) {
    player.health += 25;
    if (player.health > player.maxHealth) player.health = player.maxHealth;
}

// 붕대 복사
unique_ptr<Item> Bandage::clone() const {
    return make_unique<Bandage>(*this);
}

// 진통자 생성자
PainKiller::PainKiller() : Item(CONSUMABLE, "Pain Killer", 100) {}

// 진통제 사용 : 체력 25 회복
void PainKiller::use(Player& player) {
    player.health += 25;
    if (player.health > player.maxHealth) player.health = player.maxHealth;
}

// 진통제 복사
unique_ptr<Item> PainKiller::clone() const {
    return make_unique<PainKiller>(*this);
}

// 응급키트 생성자
MedicalKit::MedicalKit() : Item(CONSUMABLE, "Medical Kit", 200) {}

// 응급키트 사용 : 체력 50 회복
void MedicalKit::use(Player& player) {
    player.health += 50;
    if (player.health > player.maxHealth) player.health = player.maxHealth;
}

// 응급키트 복사
unique_ptr<Item> MedicalKit::clone() const {
    return make_unique<MedicalKit>(*this);
}

// 종합비타민 생성자
Multivitamin::Multivitamin() : Item(CONSUMABLE, "Multivitamin", 80) {}

// 종합비타민 사용 : 60초 간 공격력 50% 증가
void Multivitamin::use(Player& player) {
    player.increaseDamage(MULTIVITAMIN_PACK_PERCENTAGE, MULTIVITAMIN_PACK_FRAME);
}

// 종합비타민 복사
unique_ptr<Item> Multivitamin::clone() const {
    return make_unique<Multivitamin>(*this);
}

// 프로틴 생성자
Protein::Protein() : Item(CONSUMABLE, "Protein", 250) {}

// 프로틴 사용 : 현재, 최대 체력 25 상승
void Protein::use(Player& player) {
    player.maxHealth += 25;
    player.health += 25;
    if (player.health > player.maxHealth) player.health = player.maxHealth;
}

// 프로틴 복사
unique_ptr<Item> Protein::clone() const {
    return make_unique<Protein>(*this);
}

SmallMineral::SmallMineral() : Item(MINERAL, "SmallMineral", 0) {}

void SmallMineral::use(Player& player) {
    player.mineral += 5;
}

unique_ptr<Item> SmallMineral::clone() const {
    return make_unique<SmallMineral>(*this);
}

MiddleMineral::MiddleMineral() : Item(MINERAL, "MiddleMineral", 0) {}

void MiddleMineral::use(Player& player) {
    player.mineral += 10;
}

unique_ptr<Item> MiddleMineral::clone() const {
    return make_unique<MiddleMineral>(*this);
}

BigMineral::BigMineral() : Item(MINERAL, "BigMineral", 0) {}

void BigMineral::use(Player& player) {
    player.mineral += 20;
}

unique_ptr<Item> BigMineral::clone() const {
    return make_unique<BigMineral>(*this);
}