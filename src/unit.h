#pragma once
#include "includes.h"
#include "resources.h"
#include "territory.h"

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
		virtual int getAttack(Territory*, int) const { return attack; } 
		int getDefence() const { return defence; } 
		int getHP() const { return hp; } 
		int getMove() const { return move; } 
		Resources getUnitCost() const { return unitCost; } 
		int getUnitUpkeep() const { return unitUpkeep; } 
		void dealsDamage(int damage) { hp -= damage; }

		virtual int getDefenceBonus() const { return 0; }
};

class Spearman : public Unit
{
	public:
		Spearman() : Unit("Spearman", 6, 8, 30, 1, Resources({0, 0, 10, 40}), 2) {}
		UnitType getUnitType() const override { return SPEARMAN; }
		int getDefenceBonus() const override { return 25; }
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
		int getAttack(Territory*, int turn) const override; 
};

class WarElephant : public Unit
{
	public:
		WarElephant() : Unit("War Elephant", 18, 12, 60, 1, Resources({0, 0, 40, 200}), 8) {}
		UnitType getUnitType() const override { return WAR_ELEPHANT; }
		int getAttack(Territory* territory, int) const override; 
};

class UnitFactory
{
	public:
		static std::unique_ptr<Unit> createUnit(UnitType unit) 
		{
			switch (unit)
			{
				case SPEARMAN:
					return std::make_unique<Spearman>();
				case ARCHER:
					return std::make_unique<Archer>();
				case CAVALRY:
					return std::make_unique<Cavalry>();
				case WAR_ELEPHANT:
					return std::make_unique<WarElephant>();
				default:
					return nullptr;
			}
		}
};
