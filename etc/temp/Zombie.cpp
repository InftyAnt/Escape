//#include "Zombie.h"
//#include "Room.h"
//#include <queue>
//
//Zombie::Zombie(int x, int y, int speed, Room* room)
//    : position(make_pair(x, y)), speed(speed), room(room), isAlive(true), found(false), health(100) {
//    actualPosition.set(x * CELL_SIZE, y * CELL_SIZE);
//}
//
//void Zombie::update(const pair<int, int>& targetPosition, const vector<vector<bool>>& obstacles) {
//    if (!isAlive) return;
//
//    pair<int, int> nextPos = nextMove(targetPosition, obstacles);
//    if (nextPos != position) {
//        position = nextPos;
//    }
//
//    ofVec2f targetVec(position.first * CELL_SIZE, position.second * CELL_SIZE);
//    ofVec2f direction = targetVec - actualPosition;
//
//    if (direction.length() > 0) {
//        direction.normalize();
//        actualPosition += direction * speed;
//        if ((targetVec - actualPosition).length() < speed) {
//            actualPosition.set(targetVec);
//        }
//    }
//}
//
//void Zombie::draw() {
//    if (!isAlive) return;
//
//    ofSetColor(ofColor::red);
//    ofDrawCircle(actualPosition.x, actualPosition.y, CELL_SIZE / 2);
//}
//
//bool Zombie::isValidMove(int x, int y, const vector<vector<bool>>& obstacles) {
//    return x >= 0 && y >= 0 && x < obstacles.size() && y < obstacles[0].size() && !obstacles[x][y];
//}
//
//pair<int, int> Zombie::nextMove(const pair<int, int>& targetPosition, const vector<vector<bool>>& obstacles) {
//    int startX = position.first;
//    int startY = position.second;
//    int targetX = targetPosition.first;
//    int targetY = targetPosition.second;
//
//    queue<pair<int, int>> q;
//    q.push(make_pair(startX, startY));
//    vector<vector<int>> dist(obstacles.size(), vector<int>(obstacles[0].size(), -1));
//    dist[startX][startY] = 0;
//
//    vector<pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
//
//    while (!q.empty()) {
//        int x = q.front().first;
//        int y = q.front().second;
//        q.pop();
//
//        for (auto d : directions) {
//            int newX = x + d.first;
//            int newY = y + d.second;
//
//            if (isValidMove(newX, newY, obstacles) && dist[newX][newY] == -1) {
//                dist[newX][newY] = dist[x][y] + 1;
//                q.push(make_pair(newX, newY));
//
//                if (newX == targetX && newY == targetY) {
//                    break;
//                }
//            }
//        }
//    }
//
//    // Target is unreachable
//    if (dist[targetX][targetY] == -1) {
//        return position;
//    }
//
//    // Reconstruct the path
//    vector<pair<int, int>> path;
//    int x = targetX;
//    int y = targetY;
//
//    while (!(x == startX && y == startY)) {
//        path.push_back(make_pair(x, y));
//
//        for (auto d : directions) {
//            int newX = x + d.first;
//            int newY = y + d.second;
//
//            if (isValidMove(newX, newY, obstacles) && dist[newX][newY] == dist[x][y] - 1) {
//                x = newX;
//                y = newY;
//                break;
//            }
//        }
//    }
//
//    // Determine the next move based on the path
//    int horizontalDist = abs(targetX - startX);
//    int verticalDist = abs(targetY - startY);
//
//    if (horizontalDist >= verticalDist) {
//        if (targetX > startX) return make_pair(startX + 1, startY);
//        else if (targetX < startX) return make_pair(startX - 1, startY);
//    }
//
//    if (verticalDist > horizontalDist) {
//        if (targetY > startY) return make_pair(startX, startY + 1);
//        else if (targetY < startY) return make_pair(startX, startY - 1);
//    }
//
//    return path.back(); // 다음 이동 위치 반환
//}
