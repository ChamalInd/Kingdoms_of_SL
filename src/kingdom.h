#pragma once
#include "includes.h"
#include "resources.h"
#include "territory.h"
#include "events.h"

class Kingdom
{
	private:
		std::string kingdomName;
		RulerEnum rulerEnum;
		int population = 1000;
		int stability = 70;
		Resources resources = Resources({500, 200, 100, 1000});
		std::vector<Territory*> ownedTerritories;
		int buildingUpkeepDebt = 0;
		size_t armyCount = 0;
		RiceAbundance riceAbundance = ENOUGH;
		WarStatus lastWarStatus = NO_WAR;
		std::vector<int> buildingCount = {0, 0, 0, 0, 0, 0, 0, 0};

	
	public:
		Kingdom(std::string n, RulerEnum r) : kingdomName(n), rulerEnum(r) {}
		
		std::string getKingdomName() const { return kingdomName; }
		int getPopulation() const { return population; }
		int getStability() const { return stability; }
		int getNoOfTerritoriesOwned() const { return ownedTerritories.size(); }
		int getNoOfBuildingsBuilt() const; 
		size_t getArmyCount() const { return armyCount; } 
		Resources& getResourceObject() { return resources; }
		std::vector<int> getResources() const { return resources.getResources(); }
		std::vector<Territory*> getOwnedTerritories() const { return ownedTerritories; }
		int getSpecificBuildingCount(int index) const { return buildingCount[index]; }
		int getArmyUpkeep() const; 
		int getUnitCount() const; 
		void increaseArmyCount() { armyCount++; }
		
		void claimTerritory(Territory* territory, int claimCost);
		void abandonTerritory(int territoryIndex);
		void constructBuilding(Territory* territory, std::unique_ptr<Building>& building);
		void destroyBuilding(Territory* territory);
		void recruiteUnits(Territory* territory, UnitType unit, int amount, Resources totalRecruitementCost);
		void desertUnits();
		
		void collectProductions(WorldMap& worldMap, RulerEnum ruler, std::unique_ptr<Event>& currentEvent);
		void populationActivities(std::unique_ptr<Event>& currentEvent);
		void updateStability(std::unique_ptr<Event>& currentEvent);
		void buildingUpkeep();
		void armyUpkeep();
};
