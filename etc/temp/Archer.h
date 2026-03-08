#pragma once

#include "Player.h"

class Archer : public Player {
public:
	vector <Arrow*> arrows;

	Archer(Direction dir);
	~Archer();
	void firstSkill() override;
	void deleteArrow();
};