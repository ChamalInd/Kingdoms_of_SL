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

int Army::getArmyDefenceBonus() const
{
	int defenceBonus = 0;
	for (auto& U : units)
	{
		defenceBonus += U->getDefenceBonus();
	}
	return defenceBonus;
}

int Army::getArmyStrength(int defenceBonus) const
{
	defenceBonus += getArmyDefenceBonus();
	if (units.empty()) return 0;
	else return getArmyAttack() + getArmyDefence() * (100 + defenceBonus) / 100;
}

void Army::desertUnit()
{
	if (units.empty()) return;
	units.erase(units.begin());
}
		
void Army::moveArmy(Territory* territory) 
{
	std::cout << " moved Army #" << armyNumber;
	std::cout << " from " << stationedPlace->getTerritoryName() << " to " << territory->getTerritoryName() << "." << std::endl;
	stationedPlace = territory; 
}
		
void Army::getArmy() const
{
	if (units.empty()) return;
	std::vector<std::string> unitNames = {"Spearmen", "Archers", "Cavalries", "War Elephants"};
	std::vector<int> armyCounts = {0, 0, 0, 0};

	for (auto& U : units)
	{
		armyCounts[U->getUnitType()]++;
	}

	for (int i = 0; i < 4; i++) if (armyCounts[i] > 0) std::cout << armyCounts[i] << " of " << unitNames[i] << " ";
}
