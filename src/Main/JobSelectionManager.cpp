#include "JobSelectionManager.h"

// 생성자
JobSelectionManager::JobSelectionManager() : jobSelected(false), selectedJob(Job::ARCHER) {
    font.load("verdana.ttf", 24);
    ArcherImage.load("Archer.png");
    MedicImage.load("Medic.png");
    ChairmanImage.load("Chairman.png");
    FiremanImage.load("Fireman.png");
}

// 그리기
void JobSelectionManager::draw() {
    ofSetColor(255);
    int startX = 100;
    int startY = 450;
    int boxWidth = 300;
    int boxHeight = 200;
    int paddingX = 600;
    int paddingY = 100;
    int imageSize = 50;
    int sqSize = 40;
    int sqOffsetX = 200;
    int sqOffsetY = 75;
    int lineWidth = 50;
    int enterWidth = 40;
    int imgOffsetY = 40;

    font.drawString("Move : Arrow Keys", 100, 50);
    font.drawString("Attack : Q Key", 100, 100);
    font.drawString("Get Item : C Key", 100, 150);
    font.drawString("Use Item : Number Keys (1 ~ 0)", 100, 200);

    font.drawString("Press Number Key To Select Your Job!", 100, 350);

    // Archer
    int archerX = startX;
    int archerY = startY;
    font.drawString("1 : Archer", archerX, archerY);
    font.drawString("Performs piercing attacks with the longest range.", archerX, archerY + lineWidth);
    font.drawString("Difficulty :", archerX, archerY + lineWidth * 2);
    ArcherImage.draw(archerX + boxWidth - imageSize, archerY - imgOffsetY, imageSize, imageSize);
    for (int i = 0; i < 5; ++i) {
        ofNoFill();
        ofDrawRectangle(archerX + sqOffsetX + (i * imageSize), archerY + sqOffsetY, sqSize, sqSize);
        if (i < 3) {
            ofFill();
            ofDrawRectangle(archerX + sqOffsetX + (i * imageSize), archerY + sqOffsetY, sqSize, sqSize);
        }
    }

    // Medic
    int medicX = startX + boxWidth + paddingX;
    int medicY = startY;
    font.drawString("2 : Medic", medicX, medicY);
    font.drawString("Heals himself whenever he clears a room and", medicX, medicY + lineWidth);
    font.drawString("performs a single stun attack.", medicX, medicY + lineWidth + enterWidth);
    font.drawString("Difficulty :", medicX, medicY + lineWidth * 2 + enterWidth);
    MedicImage.draw(medicX + boxWidth - imageSize, medicY - imgOffsetY, imageSize, imageSize);
    for (int i = 0; i < 5; ++i) {
        ofNoFill();
        ofDrawRectangle(medicX + sqOffsetX + (i * imageSize), medicY + sqOffsetY + enterWidth, sqSize, sqSize);
        if (i < 2) {
            ofFill();
            ofDrawRectangle(medicX + sqOffsetX + (i * imageSize), medicY + sqOffsetY + enterWidth, sqSize, sqSize);
        }
    }

    // Chairman
    int chairmanX = startX;
    int chairmanY = startY + boxHeight + paddingY;
    font.drawString("3 : Chairman", chairmanX, chairmanY);
    font.drawString("Performs strong and fast melee attacks.", chairmanX, chairmanY + lineWidth);
    font.drawString("Difficulty :", chairmanX, chairmanY + lineWidth * 2);
    ChairmanImage.draw(chairmanX + boxWidth - imageSize, chairmanY - imgOffsetY, imageSize, imageSize);
    for (int i = 0; i < 5; ++i) {
        ofNoFill();
        ofDrawRectangle(chairmanX + sqOffsetX + (i * imageSize), chairmanY + sqOffsetY, sqSize, sqSize);
        if (i < 5) {
            ofFill();
            ofDrawRectangle(chairmanX + sqOffsetX + (i * imageSize), chairmanY + sqOffsetY, sqSize, sqSize);
        }
    }

    // Fireman
    int firemanX = startX + boxWidth + paddingX;
    int firemanY = startY + boxHeight + paddingY;
    font.drawString("4 : Fireman", firemanX, firemanY);
    font.drawString("Performs stun attacks on multiple enemies", firemanX, firemanY + lineWidth);
    font.drawString("based on the charged amount.", firemanX, firemanY + lineWidth + enterWidth);
    font.drawString("Difficulty :", firemanX, firemanY + lineWidth * 2 + enterWidth);
    FiremanImage.draw(firemanX + boxWidth - imageSize, firemanY - imgOffsetY, imageSize, imageSize);
    for (int i = 0; i < 5; ++i) {
        ofNoFill();
        ofDrawRectangle(firemanX + sqOffsetX + (i * imageSize), firemanY + sqOffsetY + enterWidth, sqSize, sqSize);
        if (i < 1) {
            ofFill();
            ofDrawRectangle(firemanX + sqOffsetX + (i * imageSize), firemanY + sqOffsetY + enterWidth, sqSize, sqSize);
        }
    }
    ofFill();
}

// 키 입력 감지
void JobSelectionManager::keyPressed(int key) {
    if (key >= '1' && key <= '4') {
        selectedJob = static_cast<Job>(key - '0');
        jobSelected = true;
    }
}