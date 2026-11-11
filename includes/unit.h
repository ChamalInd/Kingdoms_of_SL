#pragma once
#include "includes.h"

class Unit
{
	protected:
		std::string unitName;
		UnitType unitType;
		int attack;
		int defence;
		int hp;
		int move;
		std::vector<int> unitCost;
		int unitUpkeep;

	public:
		std::string getUnitName() const { return unitName; } 
		UnitType getUnitType() const { return unitType; } 
		int getAttack() const { return attack; } 
		int getDefence() const { return defence; } 
		int getHP() const { return hp; } 
		int getMove() const { return move; } 
		std::vector<int> getUnitCost() const { return unitCost; } 
		int getUnitUpkeep() const { return unitUpkeep; } 
};

class Spearman : public Unit
{
	public:
		Spearman()
		{
			unitName = "Spearman";
			unitType = SPEARMAN;
			attack = 6;
			defence = 8;
			hp = 30;
			move = 1;
			unitCost = {0, 0, 10, 40};
			unitUpkeep = 2;
		};
};

class Archer : public Unit
{
	public:
		Archer()
		{
			unitName = "Archer";
			unitType = ARCHER;
			attack = 9;
			defence = 4;
			hp = 20;
			move = 1;
			unitCost = {0, 0, 5, 50};
			unitUpkeep = 2;
		};
};

class Cavalry : public Unit
{
	public:
		Cavalry()
		{
			unitName = "Cavalry";
			unitType = CAVALRY;
			attack = 12;
			defence = 6;
			hp = 30;
			move = 2;
			unitCost = {0, 0, 20, 90};
			unitUpkeep = 4;
		};
};

class WarElephant : public Unit
{
	public:
		WarElephant()
		{
			unitName = "War Elephant";
			unitType = WAR_ELEPHANT;
			attack = 18;
			defence = 12;
			hp = 60;
			move = 1;
			unitCost = {0, 0, 40, 100};
			unitUpkeep = 8;
		};
};
