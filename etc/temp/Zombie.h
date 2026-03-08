//#pragma once
//
//#include "Setting.h"
//#include "ofMain.h"
//#include <vector>
//#include <utility>
//
//class Room;
//
//class Zombie {
//public:
//    bool isAlive;
//    bool found;
//    int health;
//    int speed;
//    Room* room;
//
//    pair<int, int> position;
//    ofVec2f actualPosition;
//
//    Zombie(int x, int y, int speed, Room* room);
//
//    void update(const pair<int, int>& targetPosition, const vector<vector<bool>>& obstacles);
//    void draw();
//
//    bool isValidMove(int x, int y, const vector<vector<bool>>& obstacles);
//    pair<int, int> nextMove(const pair<int, int>& targetPosition, const vector<vector<bool>>& obstacles);
//};