#pragma once
#include "includes.h"
#include "resources.h"

class Unit
{
	protected:
		std::string unitName;
		int attack;
		int defence;
		int hp;
		int move;
		Resources unitCost;
		int unitUpkeep;

	public:
		Unit(std::string n, int a, int d, int h, int m, Resources c, int u) : unitName(n), attack(a), defence(d), hp(h), move(m), unitCost(c), unitUpkeep(u) {}
		virtual ~Unit() = default;
		
		std::string getUnitName() const { return unitName; } 
		virtual UnitType getUnitType() const = 0;
		int getAttack() const { return attack; } 
		int getDefence() const { return defence; } 
		int getHP() const { return hp; } 
		int getMove() const { return move; } 
		Resources getUnitCost() const { return unitCost; } 
		int getUnitUpkeep() const { return unitUpkeep; } 
};

class Spearman : public Unit
{
	public:
		Spearman() : Unit("Spearman", 6, 8, 30, 1, Resources({0, 0, 10, 40}), 2) {}
		UnitType getUnitType() const override { return SPEARMAN; }
};

class Archer : public Unit
{
	public:
		Archer() : Unit("Archer", 9, 4, 20, 1, Resources({0, 0, 5, 50}), 2) {}
		UnitType getUnitType() const override { return ARCHER; }
};

class Cavalry : public Unit
{
	public:
		Cavalry() : Unit("Cavalry", 12, 6, 30, 2, Resources({0, 0, 20, 90}), 4) {}
		UnitType getUnitType() const override { return CAVALRY; }
};

class WarElephant : public Unit
{
	public:
		WarElephant() : Unit("War Elephant", 18, 12, 60, 1, Resources({0, 0, 40, 200}), 8) {}
		UnitType getUnitType() const override { return WAR_ELEPHANT; }
};
