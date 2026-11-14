#include "army.h"

int Army::getArmyAttack() const 
{
	if (units.empty()) return 0;
	int attackPoints = 0;
	for (auto& u : units)
	{
		if (u->getHP() <= 0) continue;
		attackPoints += u->getAttack();
	}
	return attackPoints;
}

int Army::getArmyDefence() const 
{
	if (units.empty()) return 0;
	int defencePoints = 0;
	for (auto& u : units)
	{	
		if (u->getHP() <= 0) continue;
		defencePoints += u->getDefence();
	}
	return defencePoints;
}
		
int Army::getMove() const 
{
	if (units.empty()) return 0;
	int move = INT_MAX;
	for (auto& u : units)
	{	
		if (u->getHP() <= 0) continue;
		if (u->getMove() < move) move = u->getMove(); 
	}
	return move;
}

int Army::getUpkeep() const
{
	if (units.empty()) return 0;
	int upkeep = 0;
	for (auto& u : units)
	{	
		if (u->getHP() <= 0) continue;
		upkeep += u->getUnitUpkeep(); 
	}
	return upkeep;
}

int Army::getArmyStrength(int defenceBonus) const
{
	if (units.empty()) return 0;
	else return getArmyAttack() + getArmyDefence() * (100 + defenceBonus) / 100;
}

void Army::desertUnit()
{
	if (units.empty()) return;
	units.erase(units.begin());
}
