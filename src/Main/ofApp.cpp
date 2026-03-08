#include "ofApp.h"

// 초기 설정
void ofApp::setup(){

	ofSetFullscreen(false);
	ofSetFrameRate(FPS);
	ofBackground(0);
	isGameStarted = false;

}

// 업데이트
void ofApp::update(){
	if (isGameStarted) {
		gameManager->update();
	}
}

// 그리기
void ofApp::draw(){
	if (!isGameStarted) {
		jobSelectionManager.draw();
	}
	else {
		gameManager->draw();
	}
}

// 종료
void ofApp::exit() {
	if (gameManager) {
		delete gameManager;
	}
}

// 키 입력 감지
void ofApp::keyPressed(int key){
	if (!isGameStarted) {
		jobSelectionManager.keyPressed(key);
		if (jobSelectionManager.jobSelected) {
			gameManager = new GameManager(jobSelectionManager.selectedJob);
			isGameStarted = true;
		}
	}
	else {
		gameManager->player->keyPressed(key);
	}
}

// 키 입력 해제 감지
void ofApp::keyReleased(int key) {
	if (isGameStarted) {
		gameManager->player->keyReleased(key);
	}
}