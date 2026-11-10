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
		Resources resources;
		std::vector<Territory*> ownedTerritories;
	
	public:
		Kingdom(std::string n);
		
		std::string getKingdomName() const { return kingdomName; }
		int getPopulation() const { return population; }
		int getStability() const { return stability; }
		int getNoOfTerritoriesOwned() const { return ownedTerritories.size(); }
		Resources& getResourceObject() { return resources; }
		std::vector<int> getResources() const { return resources.getResources(); }
		std::vector<Territory*> getOwnedTerritories() const { return ownedTerritories; }
		
		void claimTerritory(Territory* territory) { ownedTerritories.push_back(territory); }
		void collectProductions();
		void upkeepAndConsumption();
		void updateStability(int rate);
};
