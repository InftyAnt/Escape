#pragma once

#include "ofMain.h"
#include "GameManager.h"
#include "JobSelectionManager.h"

class ofApp : public ofBaseApp{

	public:
		GameManager* gameManager;						// 게임 매니저
		JobSelectionManager jobSelectionManager;		// 직업 선택 매니저
		bool isGameStarted;								// 게임 시작 여부

		void setup();									// 초기 설정
		void update();									// 업데이트
		void draw();									// 그리기
		void exit();									// 종료

		void keyPressed(int key);						// 키 입력 감지
		void keyReleased(int key);						// 키 입력 해제 감지
		
};
