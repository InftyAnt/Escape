#include "Archer.h"

//--------------------------------------------------------------
Archer::Archer(Direction dir) : Player(dir, ARCHER) {
	direction = dir;
}

Archer::~Archer() {
	for (auto arrow : arrows) {
		delete arrow;
	}
	arrows.clear();
}

void Archer::firstSkill() {
	Arrow* arrow = new Arrow(direction, position, 20);
}

void Archer::deleteArrow() {
	for (auto it = arrows.begin(); it != arrows.end(); ) {
		if (!(*it)->isVisible) {
			delete* it;
			it = arrows.erase(it);
		}
		else {
			it++;
		}
	}
}