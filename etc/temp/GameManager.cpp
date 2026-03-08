#include "GameManager.h"

GameManager::GameManager() : 
	player(Down, ARCHER), camera(GAMEBOARD_WIDTH * CELL_SIZE, GAMEBOARD_HEIGHT * CELL_SIZE, ROOM_SIZE * CELL_SIZE, ROOM_SIZE* CELL_SIZE), map(10, 17), currRoom(map.start), ui()  {
	
	fbo.allocate(CELL_SIZE * ROOM_SIZE, CELL_SIZE * ROOM_SIZE);
	fbo.begin();
	ofClear(255, 255, 255, 255);
	fbo.end();
}

GameManager::~GameManager() {
	fbo.destroy();
}

void GameManager::update() {
	Archer* archer = dynamic_cast <Archer*> (&player);
	if (archer) {
		for (auto& arrow : archer->arrows) {
			if (arrow->isVisible) {
				arrow->move();
			}
		}
		archer->deleteArrow();
	}
	player.move(*currRoom);
	camera.update(player);
	changeRoom();
}

void GameManager::draw() {
	fbo.begin();
	//Archer* archer = dynamic_cast<Archer*>(&player);
	//if (archer) {
	//	for (auto& arrow : archer->arrows) {
	//		arrow->draw(camera);
	//	}
	//}
	currRoom->draw();
	player.draw();
	fbo.end();
	ui.drawMinimap(*this, map);

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
}

void GameManager::changeRoom() {
	for (int i = 0; i < DIRECTION_COUNT; i++) {
		if (player.direction == i && currRoom->neighbor[i] && player.position == currRoom->doors[i]) {
			int newRow = currRoom->row + dr[i], newCol = currRoom->col + dc[i];
			currRoom = map.rooms[newRow][newCol].get();
			player.telePort(ROOM_SIZE / 2 + ROOM_SIZE / 2 * dc[(i + 2) % 4], ROOM_SIZE / 2 + ROOM_SIZE / 2 * dr[(i + 2) % 4]);
			player.updateTargetPosition();
			camera.update(player);
		}
	}
}