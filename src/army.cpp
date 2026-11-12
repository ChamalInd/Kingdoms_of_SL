#include "../includes/army.h"

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
	int move = 2;
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
	int upkeep = 2;
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
		
void Army::desertArmy()
{
	if (units.empty()) return;
	UnitType weekestUnit = NONE;
	int weekestStrength = INT_MAX;

	for (auto& u : units)
	{
		if (u->getAttack() + u->getDefence() < weekestStrength)
		{
			weekestStrength = u->getAttack() + u->getDefence();
			weekestUnit = u->getUnitType();
		}
	}

	if (weekestUnit == NONE) return;
	
	std::vector<int> weekestIndices;

	for (size_t i = 0; i < units.size(); i++)
	{
		if (units[i]->getUnitType() == weekestUnit) weekestIndices.push_back(i);
	}
	
	if (weekestIndices.empty()) return;

	for (int index : weekestIndices)
	{
		units.erase(units.begin() + index);
	}
}
