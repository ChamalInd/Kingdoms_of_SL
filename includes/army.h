#pragma once
#include "includes.h"
#include "unit.h"

class Army
{
	private:
		int armyNumber;
		std::vector<std::unique_ptr<Unit>> units;
	
	public:
		Army(int n) : armyNumber(n) {}

		int getArmyNumber() const { return armyNumber; }
		int getArmyStrength(int defenceBonus = 0) const;
		int getArmyAttack() const;
		int getArmyDefence() const; 
		int getMove() const;
		int getUpkeep() const;
		int getUnitCount() const { return units.size(); }

		void recruiteUnits(std::unique_ptr<Unit>& newUnit) { units.push_back(std::move(newUnit)); }
		void desertArmy() { units.clear(); } 
};
