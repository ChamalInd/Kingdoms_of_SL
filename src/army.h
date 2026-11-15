#pragma once
#include "includes.h"
#include "unit.h"
#include "territory.h"

class Army
{
	private:
		int armyNumber;
		std::vector<std::unique_ptr<Unit>> units;
		Territory* stationedPlace = nullptr;
	
	public:
		Army(int n, Territory* T) : armyNumber(n), stationedPlace(T) {}

		int getArmyNumber() const { return armyNumber; }
		void getArmy() const;
		Territory* getStationedPlace() const { return stationedPlace; }
		int getArmyStrength(int defenceBonus = 0) const;
		int getArmyAttack() const;
		int getArmyDefence() const; 
		int getMove() const;
		int getUpkeep() const;
		int getUnitCount() const { return units.size(); }

		void moveArmy(Territory* territory);
		void recruiteUnits(std::unique_ptr<Unit>& newUnit) { units.push_back(std::move(newUnit)); }
		void desertUnit();
};
