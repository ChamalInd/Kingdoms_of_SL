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

		int getArmyAttack() const;
		int getArmyDefence() const; 
		int getMove() const;
		int getUpkeep() const;

		void recruiteUnits(std::unique_ptr<Unit>& newUnit) { units.push_back(std::move(newUnit)); }
};
