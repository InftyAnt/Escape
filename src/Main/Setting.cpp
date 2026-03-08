#include "Setting.h"
#include "ofmain.h"

int dr[DIRECTION_COUNT] = { -1, 0, 1, 0 };
int dc[DIRECTION_COUNT] = { 0, -1, 0, 1 };

int randInt(int min, int max) {
	return static_cast <int> (ofRandom(min, max + 1));
}

int randInt(int max) {
	return static_cast <int> (ofRandom(0, max + 1));
}