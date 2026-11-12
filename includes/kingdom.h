#pragma once
#include "includes.h"
#include "resources.h"
#include "territory.h"

class Kingdom
{
	private:
		std::string kingdomName;
		int population = 1000;
		int stability = 70;
		Resources resources = Resources({500, 200, 100, 1000});
		std::vector<Territory*> ownedTerritories;
		int buildingUpkeepDebt = 0;
		int armyCount = 0;
		std::vector<int> buildingCount = {0, 0, 0, 0, 0, 0, 0, 0};
	
	public:
		Kingdom(std::string n);
		
		std::string getKingdomName() const { return kingdomName; }
		int getPopulation() const { return population; }
		int getStability() const { return stability; }
		int getNoOfTerritoriesOwned() const { return ownedTerritories.size(); }
		int getNoOfBuildingsBuilt() const; 
		int getArmyCount() const { return armyCount; } 
		Resources& getResourceObject() { return resources; }
		std::vector<int> getResources() const { return resources.getResources(); }
		std::vector<Territory*> getOwnedTerritories() const { return ownedTerritories; }
		int getSpecificBuildingCount(int index) const { return buildingCount[index]; }
		
		void claimTerritory(Territory* territory) { ownedTerritories.push_back(territory); }
		void increaseSpecificBuildingCount(int index) { buildingCount[index]++; }
		void increaseArmyCount() { armyCount++; }
		void collectProductions(WorldMap& worldMap, RulerEnum ruler);
		void populationActivities();
		void buildingUpkeep();
		void armyUpkeep();
		void updateStability(int rate);
};
