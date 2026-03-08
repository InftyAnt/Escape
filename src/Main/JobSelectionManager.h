#pragma once

#include "ofMain.h"
#include "Setting.h"
#include "GameManager.h"
#include "../Object/Player.h"

// 직업 선택 매니저 : 처음 직업 선택 관리
class JobSelectionManager {
public:
    bool jobSelected;           // 직업 선택 여부
    Job selectedJob;            // 선택된 직업
    ofTrueTypeFont font;        // 폰트
    ofImage ArcherImage;        // 아처 이미지
    ofImage MedicImage;         // 메딕 이미지
    ofImage ChairmanImage;      // 체어멘 이미지
    ofImage FiremanImage;       // 파이어맨 이미지

    JobSelectionManager();      // 생성자
    void draw();                // 그리기
    void keyPressed(int key);   // 키 입력 감지
};
