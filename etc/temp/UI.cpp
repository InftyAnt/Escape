#include "UI.h"
#include "GameManager.h"

UI::UI() {

}

UI::~UI() {

}

void UI::drawMinimap(const GameManager& gameManager, const Map& map) {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (map.rooms[i][j]->isAvailable) {
				if (map.rooms[i][j].get() == map.boss) {
					ofSetColor(ofColor::red);
				}
				else if (map.rooms[i][j].get() == map.portal) {
					ofSetColor(255, 0, 255);
				}
				else {
					ofSetColor(127);
				}
				ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE, MINIMAP_SIZE, MINIMAP_SIZE);
				if (map.rooms[i][j].get() == gameManager.currRoom) {
					ofSetColor(ofColor::black);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + (MINIMAP_SIZE - CURR_ROOM_INDICATOR_SIZE) / 2, MINIMAP_Y + i * MINIMAP_CELL_SIZE + (MINIMAP_SIZE - CURR_ROOM_INDICATOR_SIZE) / 2, CURR_ROOM_INDICATOR_SIZE, CURR_ROOM_INDICATOR_SIZE);
				}
				if (map.rooms[i][j]->neighbor[2]) {
					ofSetColor(127);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + MINIMAP_SIZE / 2 - MINIMAP_GAP / 2, MINIMAP_Y + i * MINIMAP_CELL_SIZE + MINIMAP_SIZE, MINIMAP_GAP, MINIMAP_GAP);
				}
				if (map.rooms[i][j]->neighbor[3]) {
					ofSetColor(127);
					ofDrawRectangle(MINIMAP_X + j * MINIMAP_CELL_SIZE + MINIMAP_SIZE, MINIMAP_Y + i * MINIMAP_CELL_SIZE + MINIMAP_SIZE / 2 - MINIMAP_GAP / 2, MINIMAP_GAP, MINIMAP_GAP);
				}
				ofSetColor(255);
			}
		}
	}
}