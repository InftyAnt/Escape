#include "UI.h"
#include "GameManager.h"
#include "../Object/Player.h"
#include "../Object/Zombie.h"
#include "../Map/Map.h"

// 생성자
UI::UI() {
	font15.load("verdana.ttf", 15);
	fontb15.load("verdanab.ttf", 15);
}

// 소멸자
UI::~UI() {

}

// 미니맵 그리기
void UI::drawMinimap(const GameManager& gameManager, const Map& map) {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (map.rooms[i][j]->isAvailable) {
				// 보스 방은 빨간색, 포탈 방은 자주색, 시작 방은 하늘색, 일반 방은 회색으로 표시
				if (map.rooms[i][j]->isBoss) {
					ofSetColor(ofColor::red);
				}
				else if (map.rooms[i][j]->isPortal) {
					ofSetColor(255, 0, 255);
				}
				else if (map.rooms[i][j]->isTreasure || map.rooms[i][j]->isShop) {
					ofSetColor(0, 255, 0);
				}
				else if (map.rooms[i][j]->isStart) {
					ofSetColor(0, 255, 255);
				}
				else {
					ofSetColor(127);
				}
				ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE, MINIMAP_SIZE, MINIMAP_SIZE);

				// 현재 방은 가운데에 검은 정사각형을 그림
				if (map.rooms[i][j].get() == gameManager.currRoom) {
					ofSetColor(ofColor::black);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + (MINIMAP_SIZE - CURR_ROOM_INDICATOR_SIZE) / 2, MINIMAP_Y + i * MINIMAP_CELL_SIZE + (MINIMAP_SIZE - CURR_ROOM_INDICATOR_SIZE) / 2, CURR_ROOM_INDICATOR_SIZE, CURR_ROOM_INDICATOR_SIZE);
				}
				
				// 다른 방과 연결되어 있으면 연결 통로 그림
				if (map.rooms[i][j]->neighbor[2]) {
					ofSetColor(127);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + MINIMAP_SIZE / 2 - MINIMAP_GAP / 2, MINIMAP_Y + i * MINIMAP_CELL_SIZE + MINIMAP_SIZE, MINIMAP_GAP, MINIMAP_GAP);
				}
				if (map.rooms[i][j]->neighbor[3]) {
					ofSetColor(127);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + MINIMAP_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE + MINIMAP_SIZE / 2 - MINIMAP_GAP / 2, MINIMAP_GAP, MINIMAP_GAP);
				}

				// 클리어된 방은 초록색 띠를 그림
				if (map.rooms[i][j]->isClear && !map.rooms[i][j]->isStart && !map.rooms[i][j]->isPortal) {
					ofSetColor(0, 255, 0);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE, MINIMAP_SIZE, MINIMAP_W);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE, MINIMAP_W, MINIMAP_SIZE);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE + MINIMAP_SIZE - MINIMAP_W, MINIMAP_SIZE, MINIMAP_W);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + MINIMAP_SIZE - MINIMAP_W, MINIMAP_Y + i * MINIMAP_CELL_SIZE, MINIMAP_W, MINIMAP_SIZE);
				}

				ofSetColor(255);
			}
		}
	}
}

// 플레이어의 체력바 그리기
void UI::drawHealthBar(const Player& player) {
	int barLength = HEALTHBAR_L * player.health / player.maxHealth;
	ofSetColor(127);
	ofDrawRectangle(HEALTHBAR_X, HEALTHBAR_Y - HEALTHBAR_STROKE_W, HEALTHBAR_L + 2 * HEALTHBAR_STROKE_W, HEALTHBAR_W + 2 * HEALTHBAR_STROKE_W);
	ofSetColor(ofColor::red);
	ofDrawRectangle(HEALTHBAR_X + HEALTHBAR_STROKE_W, HEALTHBAR_Y, barLength, HEALTHBAR_W);
	ofSetColor(255);

	// 체력 텍스트 표시
	string healthText = to_string(player.health) + " / " + to_string(player.maxHealth);
	ofRectangle textBox = font15.getStringBoundingBox(healthText, 0, 0);
	float textX = HEALTHBAR_X + (HEALTHBAR_L + 2 * HEALTHBAR_STROKE_W - textBox.getWidth()) / 2;
	float textY = HEALTHBAR_Y + (HEALTHBAR_W + textBox.getHeight()) / 2 - textBox.getHeight() / 5;
	font15.drawString(healthText, textX, textY);
}

// 스킬의 쿨타임 그리기
void UI::drawCool(const Player& player) {
	// 원형의 중심 좌표와 반지름 설정
	int centerX = RIGHT_MIDDLE_X;
	int centerY = ofGetHeight() - 150;
	int outerRadius = 80;
	int innerRadius = 65;
	float startAngle = -90;  // 시작 각도(위쪽)
	float endAngle = startAngle - 360 * player.firstCoolCount / player.firstCool;  // 종료 각도(쿨타임 퍼센트에 따라 달라짐)

	// 배경 흰색 원형 띠 그림
	ofSetColor(127);
	ofBeginShape();
	for (float angle = startAngle; angle >= startAngle - 360; angle -= 1) {
		float x = centerX + outerRadius * cos(ofDegToRad(angle));
		float y = centerY + outerRadius * sin(ofDegToRad(angle));
		ofVertex(x, y);
	}
	for (float angle = startAngle - 360; angle <= startAngle; angle += 1) {
		float x = centerX + innerRadius * cos(ofDegToRad(angle));
		float y = centerY + innerRadius * sin(ofDegToRad(angle));
		ofVertex(x, y);
	}
	ofEndShape(true);

	// 쿨타임 빨간색 원형 띠 그림
	ofSetColor(ofColor::red);
	ofBeginShape();
	for (float angle = startAngle; angle >= endAngle; angle -= 1) {
		float x = centerX + outerRadius * cos(ofDegToRad(angle));
		float y = centerY + outerRadius * sin(ofDegToRad(angle));
		ofVertex(x, y);
	}
	for (float angle = endAngle; angle <= startAngle; angle += 1) {
		float x = centerX + innerRadius * cos(ofDegToRad(angle));
		float y = centerY + innerRadius * sin(ofDegToRad(angle));
		ofVertex(x, y);
	}
	ofEndShape(true);

	ofSetColor(255);
}

// 플레이어의 인벤토리 그리기
void UI::drawInventory(const Player& player) {
	int x = 20;
	int y = 60;
	int w = 50;
	int sw = 5;

	ofSetColor(127);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			ofDrawRectangle(x + i * w, y + j * w, w + sw, sw);
			ofDrawRectangle(x + i * w, y + j * w, sw, w + sw);
			ofDrawRectangle(x + i * w, y + (j + 1) * w, w + sw, sw);
			ofDrawRectangle(x + (i + 1) * w, y + j * w, sw, w + sw);
		}
	}
	ofSetColor(255);
	int i = 0;
	for (const auto& item : player.inventory) {
		if (item) {
			// 사진 길이 맞추기
			int originalWidth = item->image.getWidth();
			int originalHeight = item->image.getHeight();
			float aspectRatio = static_cast<float>(originalWidth) / static_cast<float>(originalHeight);

			int drawWidth, drawHeight;
			if (originalWidth > originalHeight) {
				drawWidth = w - sw;
				drawHeight = static_cast<int>((w - sw) / aspectRatio);
			}
			else {
				drawHeight = w;
				drawWidth = static_cast<int>((w - sw) * aspectRatio);
			}

			item->image.draw(x + w * (i % 5) + (w + sw - drawWidth) / 2, y + w * (i / 5) + (w + sw - drawHeight) / 2, drawWidth, drawHeight);
		}
		i++;
	}
}

void UI::drawMineral(const Player& player) {
	string mineralImagePath = "MiddleMineral.png";
	ofImage mineralImage;
	if (!mineralImage.load(mineralImagePath)) {
		ofLogError("Item") << "Failed to load image: " << mineralImagePath;
	}
	int originalWidth = mineralImage.getWidth();
	int originalHeight = mineralImage.getHeight();
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
	mineralImage.draw(MINERAL_IMAGE_X, MINERAL_IMAGE_Y - drawHeight / 2, drawWidth, drawHeight);
	string mineralText = to_string(player.mineral);
	ofRectangle textBox = font15.getStringBoundingBox(mineralText, 0, 0);
	font15.drawString(mineralText, MINERAL_TEXT_X, MINERAL_TEXT_Y + textBox.getHeight() / 2);
}

void UI::showRoomZombieState(const GameManager& gameManager) {
	string state;
	if (gameManager.currRoom->isClear) {
		ofSetColor(0, 255, 0);
		state = "Room Clear";
	}
	else {
		ofSetColor(255, 0, 0);
		state = "Remaining Zombies : " + to_string(gameManager.currRoom->zombies.size()) + " / " + to_string(gameManager.currRoom->zombieCnt);
	}
	fontb15.drawString(state, SHOW_ROOM_ZOMBIE_STATE_X - fontb15.stringWidth(state) / 2, SHOW_ROOM_ZOMBIE_STATE_Y);
	ofSetColor(255);
}