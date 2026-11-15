#include "army.h"

int Army::getArmyAttack(int turn) const 
{
	if (units.empty()) return 0;
	int attackPoints = 0;
	for (auto& u : units)
	{
		if (u->getHP() <= 0) continue;
		attackPoints += u->getAttack(stationedPlace, turn);
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
	else return getArmyAttack(0) + getArmyDefence() * (100 + defenceBonus) / 100;
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
	previousPlace = stationedPlace;
	stationedPlace = territory; 
}

void Army::goBack()
{
	if (units.empty()) return;
	std::cout << " moved Army #" << armyNumber;
	std::cout << " from " << stationedPlace->getTerritoryName() << " to " << previousPlace->getTerritoryName() << "." << std::endl;
	stationedPlace = previousPlace; 
	previousPlace = nullptr;
}
		
int Army::getArmyDamage(int defend, int turn) const
{
	int attack = getArmyAttack(turn) * (rand() % 41 + 80) / 100;
	return std::max(1, (attack - defend / 2));
}
		
void Army::dealsDamage(int damage, int turn) 
{
	if (units.empty()) return;
	for (auto& U : units)
	{
		if (damage > U->getHP())
		{
			damage -= U->getHP();
			if (!(turn == 1 && U->getUnitType() == ARCHER)) U->dealsDamage(U->getHP());
		}
		else if (damage > 0) 
		{
			U->dealsDamage(damage);
			break;
		}
		else break;
	}

	for (size_t i = 0; i < units.size(); i++)
	{
		if (units[i]->getHP() <= 0)
		{
			units.erase(units.begin() + i);
			i--;
		}
	}
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
