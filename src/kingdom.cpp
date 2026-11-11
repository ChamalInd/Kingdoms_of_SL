#include "../includes/kingdom.h"

Kingdom::Kingdom(std::string n)
{
	kingdomName = n;
}

int Kingdom::getNoOfBuildingsBuilt() const
{
	int buildingCount = 0;
	for (Territory* T : ownedTerritories)
	{
		if (T->getTerritoryBuilding() != nullptr) buildingCount++;
	}
	return buildingCount;
}

void Kingdom::collectProductions(WorldMap& worldMap)
{
	std::vector<int> resourceIncrements = {0, 0, 0, 0};
	for (Territory* T : ownedTerritories)
	{
		std::vector<int> baseProduction = T->getTerritoryProduction();
		for (int i = 0; i < 4; i++) 
		{
			int reduction = 0;
			if (stability < 30)
			{
				reduction = baseProduction[i] * 25 / 100;
			}
			resourceIncrements[i] += (baseProduction[i] - reduction);
		}
		if (T->getTerritoryBuilding() != nullptr && T->getTerritoryBuilding()->getBuildingStatus() == OPEN)
		{
			BuildingEffect effect = T->getTerritoryBuilding()->performEffect(T, worldMap);
			for (int i = 0; i < 4; i++) resourceIncrements[i] += effect.resourceIncrement[i];
			updateStability(effect.stabilityIncrement);
		}
	}

	resources += resourceIncrements;

	std::cout << kingdomName << " produced: Gold +" << resourceIncrements[3] << ", Rice +";
	std::cout << resourceIncrements[0] << ", Timber +" << resourceIncrements[1] << ", Iron +" << resourceIncrements[2] << "." << std::endl << std::endl;
}
		
void Kingdom::upkeepAndConsumption()
{
	std::vector<int> resourceIncrement = {0, 0, 0, 0};
	std::vector<int> currentResources = resources.getResources();

	// consume rice
	int consumption = population / 20;

	if (consumption <= currentResources[0])
	{
		resourceIncrement[0] = -consumption;
	}

	if ((consumption * 2) <= currentResources[0])
	{
		population += population * 2 / 100;
		updateStability(1);
	}
	
	if (consumption > currentResources[0])
	{
		resourceIncrement[0] = 0 - currentResources[0];
		population -= population * 5 / 100;
		updateStability(-10);
	}

	// taxes
	resourceIncrement[3] = population / 25;

	resources += resourceIncrement;
	
	std::cout << kingdomName << " population consumed: " << std::abs(resourceIncrement[0]) << " Rice." << std::endl;
	std::cout << kingdomName << " population paid taxes: " << resourceIncrement[3] << " Gold." << std::endl << std::endl;
	
	// upkeeps
	resourceIncrement = {0, 0, 0, 0};
	currentResources = resources.getResources();
	
	int goldUpkeep = 0;
	int highestGoldUpkeep = 0;
	int highestGoldUpkeepIndex = 0;

	for (size_t i = 0; i < ownedTerritories.size(); i++)
	{
		if (ownedTerritories[i]->getTerritoryBuilding() == nullptr) continue;
		int upkeep = ownedTerritories[i]->getTerritoryBuilding()->getUpkeepCost();
		goldUpkeep += upkeep;
		if (upkeep > highestGoldUpkeep && ownedTerritories[i]->getTerritoryBuilding()->getBuildingStatus() == OPEN)
		{
			highestGoldUpkeep = upkeep;
			highestGoldUpkeepIndex = i; 
		}
	}

	if ((goldUpkeep + buildingUpkeepDebt) <= currentResources[3])
	{
		resourceIncrement[3] = -(goldUpkeep + buildingUpkeepDebt);
		std::cout << kingdomName << " paid building upkeep: " << std::abs(resourceIncrement[3]) << " Gold." << std::endl;	
	
		if (buildingUpkeepDebt > 0) std::cout << kingdomName << " cleared there building upkeep debt." << std::endl;

		buildingUpkeepDebt = 0;
		
		for (Territory* T : ownedTerritories)
		{
			if (T->getTerritoryBuilding() == nullptr) continue;
			if (T->getTerritoryBuilding()->getBuildingStatus() == OPEN) continue;
			T->getTerritoryBuilding()->setBuildingStatus(OPEN);
			std::cout << "\t" << T->getTerritoryBuilding()->getBuildingName() << " on " << T->getTerritoryName() << " is now open." << std::endl;
		}

	}
	else
	{
		resourceIncrement[3] = -currentResources[3];
		ownedTerritories[highestGoldUpkeepIndex]->getTerritoryBuilding()->setBuildingStatus(CLOSE);
		std::cout << kingdomName << " not enough Gold to pay upkeep of buildings." << std::endl;
		buildingUpkeepDebt = buildingUpkeepDebt + goldUpkeep + currentResources[3];
		std::cout << "\t" << ownedTerritories[highestGoldUpkeepIndex]->getTerritoryBuilding()->getBuildingName();
		std::cout << " on " << ownedTerritories[highestGoldUpkeepIndex]->getTerritoryName() << " is now closed due to upkeep debt." << std::endl;
	}
	
	resources += resourceIncrement;
	
}

void Kingdom::updateStability(int rate)
{
	stability = std::clamp(stability + rate, 0, 100);

	if (stability <= 0)
	{
		if (getNoOfTerritoriesOwned() > 1)
		{
			int randomTerritory = rand() % (getNoOfTerritoriesOwned() - 1) + 1;
			std::cout << "Stability of " << kingdomName << " has reached 0. Rebellion in ";
			std::cout << ownedTerritories[randomTerritory]->getTerritoryName() << "! " << kingdomName << " loses control." << std::endl;
			ownedTerritories[randomTerritory]->setTerritoryRuler(NEUTRAL);
			ownedTerritories.erase(ownedTerritories.begin() + randomTerritory);
		}
		stability = 30;
	}
}
