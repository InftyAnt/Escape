#include "Room.h"

//--------------------------------------------------------------
Room::Room(int row, int col) : row(row), col(col), width(ROOM_SIZE), height(ROOM_SIZE), isAvailable(false), isIncluded(false), isNear(false), isStart(false), isBoss(false), isPortal(false), neighbor(vector <bool> (DIRECTION_COUNT, false)), roomState(vector <vector <int>> (height, vector <int> (width, 0))) {
    
}

//--------------------------------------------------------------
Room::~Room() {

}

//--------------------------------------------------------------
void Room::setCircle(vector <pair <int, int>>& circle, int radius) {
    for (int i = -radius; i <= radius; ++i) {
        int abs_i = abs(i);
        int remaining = radius - abs_i;

        circle.push_back({ width / 2 + i, height / 2 + remaining });
        if (remaining != 0) {
            circle.push_back({ width / 2 + i, height / 2 - remaining });
        }
    }
}

//--------------------------------------------------------------
void Room::setWalls() {
    int randomRoomType = static_cast<int>(ofRandom(1, MAP_TYPE_COUNT));
    if (isStart) randomRoomType = 1;
    else if (isPortal || isBoss) randomRoomType = 0;
    for (int i = 0; i < ROOM_SIZE; i++) {
        roomState[i][0] = 1;
        roomState[0][i] = 1;
        roomState[i][ROOM_SIZE - 1] = 1;
        roomState[ROOM_SIZE - 1][i] = 1;
    }
    switch (randomRoomType) {
    case Empty :
        break;
    case OneInOne:
        {
            vector <vector <pair <int, int>>> circles;
            circles.resize(3);

            setCircle(circles[0], 2);
            setCircle(circles[1], 5);
            setCircle(circles[2], 9);

            for (int i = 0; i < 3; i++) {
                if (!circles.at(i).empty()) {
                    int removeIdx = rand() % circles.at(i).size();
                    circles.at(i).at(removeIdx) = circles.at(i).back();
                    circles.at(i).pop_back();
                }
            }

            for (auto circle : circles) {
                for (auto dot : circle) {
                    roomState[dot.first][dot.second] = 1;
                }
            }
        }
        break;
    case Tetris :
        {
            vector <vector <pair <int, int>>> mino;
            int x_offset = static_cast <int> (ofRandom(2, 7));
            int y_offset = static_cast <int> (ofRandom(2, 7));
            mino.resize(4);
            for (int i = 0; i < 4; i++) {
                mino[i].resize(4);
                for (int j = 0; j < 4; j++) {
                    int posX = 5 * i + x_offset;
                    int posY = 5 * j + y_offset;
                    switch (static_cast <int> (ofRandom(7))) {
                    case 0:
                        switch (static_cast <int> (ofRandom(4))) {
                        case 0:
                            f(posX, posY, 1, 0);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        case 1:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 3, 1);
                            break;
                        case 2:
                            f(posX, posY, 2, 0);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 2, 3);
                            break;
                        case 3:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 3, 2);
                            break;
                        }
                        break;
                    case 1:
                        f(posX, posY, 1, 1);
                        f(posX, posY, 1, 2);
                        f(posX, posY, 2, 1);
                        f(posX, posY, 2, 2);
                        break;
                    case 2:
                        switch (static_cast <int> (ofRandom(4))) {
                        case 0:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 2);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 2);
                            break;
                        case 3:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 3:
                        switch (static_cast <int> (ofRandom(4))) {
                        case 0:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 3);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 3:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 4:
                        switch (static_cast <int> (ofRandom(4))) {
                        case 0:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 1);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 2, 3);
                            break;
                        case 3:
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 5:
                        switch (static_cast <int> (ofRandom(4))) {
                        case 0:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 1);
                            break;
                        case 1:
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            f(posX, posY, 2, 3);
                            break;
                        case 2:
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 2);
                            break;
                        case 3:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            break;
                        }
                        break;
                    case 6:
                        switch (static_cast <int> (ofRandom(4))) {
                        case 0:
                            f(posX, posY, 0, 1);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 2, 2);
                            break;
                        case 1:
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 1);
                            f(posX, posY, 2, 2);
                            break;
                        case 2:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 1, 2);
                            f(posX, posY, 1, 3);
                            f(posX, posY, 2, 3);
                            break;
                        case 3:
                            f(posX, posY, 0, 2);
                            f(posX, posY, 0, 3);
                            f(posX, posY, 1, 1);
                            f(posX, posY, 1, 2);
                            break;
                        }
                        break;
                    }
                }
            }
            break;
        } 
        break;
    case Gaal :
        {
            f(4, 8);
            f(4, 7);
            f(5, 7);
            f(6, 7);
            f(6, 6);
            f(7, 6);
            f(8, 6);
            f(9, 6);
            f(10, 6);
            f(10, 5);
            f(11, 5);
            f(12, 5);
            f(13, 5);
            f(13, 6);
            f(12, 6);
            f(12, 7);
            f(12, 8);
            f(11, 8);
            f(11, 9);
            f(11, 10);
            f(10, 10);
            f(10, 11);
            f(9, 11);
            f(9, 12);
            f(8, 12);
            f(8, 13);
            f(7, 13);
            f(7, 14);
            f(6, 14);

            for (int i = 2; i <= 12; i++) {
                f(15, i);
            }
            f(16, 8);
            f(17, 7);

            f(8, 16);
            f(9, 16);
            f(10, 16);
            f(10, 15);
            f(11, 15);
            f(12, 15);
            f(13, 14);
            f(14, 14);
            f(15, 14);
            f(15, 15);
            f(15, 16);
            f(15, 17);
            f(14, 17);
            f(13, 18);
            f(12, 18);
            f(11, 18);
            f(10, 18);
            f(9, 19);
            f(9, 20);
            f(9, 21);
            f(9, 22);
            f(10, 22);
            f(11, 22);
            f(12, 21);
            f(13, 21);
            f(14, 20);
            f(15, 20);
            f(15, 19);

            for (int i = 2; i <= 17; i++) {
                f(20, i);
            }
            f(20, 20);
        }
        break;
    case X :
        {
            for (int i = 4; i < ROOM_SIZE - 4; i++) {
                f(i, i);
                f(i, ROOM_SIZE - 1 - i);
            }
        }
        break;
    case ZigZag :
        {
            for (int i = 2; i < ROOM_SIZE; i += 4) {
                if (i == ROOM_SIZE / 2) continue;
                for (int j = 1; j <= ROOM_SIZE / 3; j++) {
                    f(i, j);
                    f(i, ROOM_SIZE - 1 - j);
                }
            }
            for (int i = 4; i < ROOM_SIZE; i += 4) {
                if (i == ROOM_SIZE / 2) continue;
                for (int j = ROOM_SIZE / 3; j <= ROOM_SIZE / 2; j++) {
                    f(i, j);
                    f(i, ROOM_SIZE - 1 - j);
                }
            }
            break;
        }
    default :
        break;
    }
}

//--------------------------------------------------------------
void Room::setDoors() {
    doors.resize(4);
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        doors[i].first = ROOM_SIZE / 2 + ROOM_SIZE / 2 * dc[i];
        doors[i].second = ROOM_SIZE / 2 + ROOM_SIZE / 2 * dr[i];
        if (neighbor[i]) roomState[doors[i].first][doors[i].second] = 0;
    }
}

//--------------------------------------------------------------
//void Room::setZombies() {
//    zombies.resize(5);
//    for (int i = 0; i < 5; i++) {
//        int p, q;
//        do {
//            p = static_cast <int> (ofRandom(2, ROOM_SIZE - 2));
//            q = static_cast <int> (ofRandom(2, ROOM_SIZE - 2));
//        } while (roomState[p][q] != 0);
//        zombies.push_back(make_unique <Zombie> (p, q, 5, this));
//    }
//}

//--------------------------------------------------------------
void Room::draw() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (roomState[i][j] == 1) {
                ofSetColor(ofColor::red);
            }
            else {
                ofSetColor(ofColor::green);
            }
            ofDrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
    }

    ofSetColor(ofColor::black);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            ofDrawRectangle(i * CELL_SIZE, j * CELL_SIZE, GRID_LINE_WIDTH, CELL_SIZE);
            ofDrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, GRID_LINE_WIDTH);
            ofDrawRectangle((i + 1) * CELL_SIZE - GRID_LINE_WIDTH, j * CELL_SIZE, GRID_LINE_WIDTH, CELL_SIZE);
            ofDrawRectangle(i * CELL_SIZE, (j + 1) * CELL_SIZE - GRID_LINE_WIDTH, CELL_SIZE, GRID_LINE_WIDTH);
        }
    }
}

void Room::f(int i, int j) {
    roomState[i][j] = 1;
}

void Room::f(int posX, int posY, int i, int j) {
    roomState[posX + i][posY + j] = 1;
}

void Room::f(int i, int j, int code) {
    roomState[i][j] = code;
}

void Room::f(int posX, int posY, int i, int j, int code) {
    roomState[posX + i][posY + j] = code;
}