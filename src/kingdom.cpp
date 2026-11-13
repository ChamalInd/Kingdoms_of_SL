#include "kingdom.h"

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

int Kingdom::getUnitCount() const
{
	int unitCount = 0;
	for (Territory* T : ownedTerritories)
	{
		if (T->getTerritoryArmy() == nullptr) continue;
		unitCount += T->getTerritoryArmy()->getUnitCount();
	}
	return unitCount;
}

int Kingdom::getArmyUpkeep() const
{
	int totalRiceUpkeep = 0;
	for (size_t i = 0; i < ownedTerritories.size(); i++)
	{
		Territory* T = ownedTerritories[i]; 
		if (T->getTerritoryArmy() == nullptr) continue;
		totalRiceUpkeep += T->getTerritoryArmy()->getUpkeep(); 
	}

	return totalRiceUpkeep;
}

void Kingdom::collectProductions(WorldMap& worldMap, RulerEnum ruler, std::unique_ptr<Event>& currentEvent)
{
	Resources resourceIncrements = Resources({0, 0, 0, 0});
	int stabilityIncrement = 0;
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
			resourceIncrements.addToStock(i, (baseProduction[i] - reduction));
		}
		if (T->getTerritoryBuilding() != nullptr && T->getTerritoryBuilding()->getBuildingStatus() == OPEN)
		{
			BuildingEffect effect = T->getTerritoryBuilding()->performEffect(T, worldMap, ruler);
			for (int i = 0; i < 4; i++) 
			{
				if (stability < 30) resourceIncrements.addToStock(i, effect.resourceIncrement[i] * 75 / 100);
				else resourceIncrements.addToStock(i, effect.resourceIncrement[i]);
			}
			stabilityIncrement += effect.stabilityIncrement;
		}
	}

	if (currentEvent != nullptr) 
	{
		switch (currentEvent->getEventType())
		{ 
			case DROUGHT:
				resourceIncrements.subFromStock(RICE, resourceIncrements.getRiceStock() / 2);
				break;
			case GOOD_MONSOON:
				resourceIncrements.addToStock(RICE, resourceIncrements.getRiceStock() / 2);
				break;
			
			case F_O_IRON_AGE:
				resourceIncrements.subFromStock(IRON, resourceIncrements.getIronStock() / 2);
				break;
			default:
				break;
		}
	}

	resources += resourceIncrements;
	updateStability(stabilityIncrement);

	std::cout << kingdomName << " produced: Gold +" << resourceIncrements.getGoldStock() << ", Rice +" << resourceIncrements.getRiceStock();
	std::cout << ", Timber +" << resourceIncrements.getTimberStock() << ", Iron +" << resourceIncrements.getIronStock() << "." << std::endl << std::endl;
}
		
void Kingdom::populationActivities(std::unique_ptr<Event>& currentEvent)
{
	int riceConsumption = 0; 
	int taxIncome = 0;
	int currentRiceStock = resources.getRiceStock();	

	
	if (currentEvent != nullptr)
	{
		switch (currentEvent->getEventType())
		{
			case PLAGUE:
				population -= population * 10 / 100;
				updateStability(-5);
				break;
			case ROYAL_FESTIVAL:
				updateStability(10);
				break;
			default:
				break;
		}
	}

	// consume rice
	int consumption = population / 20;

	if (currentEvent != nullptr && currentEvent->getEventType() == ROYAL_FESTIVAL) consumption += consumption * 50 / 100;

	if (consumption <= currentRiceStock)
	{
		riceConsumption = consumption;
	}

	if ((consumption * 2) <= currentRiceStock)
	{
		population += population * 2 / 100;
		updateStability(1);
	}
	
	if (consumption > currentRiceStock)
	{
		riceConsumption = currentRiceStock;
		population -= population * 5 / 100;
		updateStability(-10);
		std::cout << kingdomName << " is starving! Population falls by 5%. Stability - 10." << std::endl;
	}

	// taxes
	taxIncome = population / 25;

	resources.subFromStock(RICE, riceConsumption);
	resources.addToStock(GOLD, taxIncome);
	
	std::cout << kingdomName << " population consumed: " << riceConsumption << " Rice." << std::endl;
	std::cout << kingdomName << " population paid taxes: " << taxIncome << " Gold." << std::endl << std::endl;

}
		
void Kingdom::buildingUpkeep()
{
	int currentGoldStock = resources.getGoldStock();
	int upkeepCost = 0;

	int goldUpkeep = 0;
	int highestGoldUpkeep = 0;
	int highestGoldUpkeepIndex = -1;

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

	if ((goldUpkeep + buildingUpkeepDebt) <= currentGoldStock)
	{
		upkeepCost = goldUpkeep + buildingUpkeepDebt;
		std::cout << kingdomName << " paid building upkeep: " << upkeepCost << " Gold." << std::endl;	
	
		if (buildingUpkeepDebt > 0) std::cout << kingdomName << " cleared their building upkeep debt." << std::endl;

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
		upkeepCost = currentGoldStock;
		buildingUpkeepDebt += goldUpkeep - upkeepCost;
		std::cout << kingdomName << " does not enough Gold to pay upkeep of buildings." << std::endl;
		if (highestGoldUpkeepIndex != -1) 
		{
			ownedTerritories[highestGoldUpkeepIndex]->getTerritoryBuilding()->setBuildingStatus(CLOSE);
			std::cout << "\t" << ownedTerritories[highestGoldUpkeepIndex]->getTerritoryBuilding()->getBuildingName();
			std::cout << " on " << ownedTerritories[highestGoldUpkeepIndex]->getTerritoryName() << " is now closed due to upkeep debt." << std::endl;
		}
	}
	
	resources.subFromStock(GOLD, upkeepCost);
	
}

void Kingdom::armyUpkeep()
{
	int riceStock = resources.getRiceStock();
	std::vector<int> armiesTried;

	while (true)
	{
		int totalRiceUpkeep = 0;
		int armyStrength = INT_MAX;
		int weekestArmy = -1;
		
		for (size_t i = 0; i < ownedTerritories.size(); i++)
		{
			Territory* T = ownedTerritories[i]; 
			if (T->getTerritoryArmy() == nullptr) continue;
			if (T->getTerritoryArmy()->getUnitCount() <= 0) continue;
			totalRiceUpkeep += T->getTerritoryArmy()->getUpkeep(); 
			if (armyStrength > T->getTerritoryArmy()->getArmyStrength())
			{
				armyStrength = T->getTerritoryArmy()->getArmyStrength();
				weekestArmy = i;
			}
		}

		if (weekestArmy == -1) break;
		if (totalRiceUpkeep == 0) break;
		
		auto found = std::find(armiesTried.begin(), armiesTried.end(), ownedTerritories[weekestArmy]->getTerritoryArmy()->getArmyNumber());
		if (found != armiesTried.end())
		{
			if (armiesTried.size() == armyCount) break;
			else continue;
		}
		else armiesTried.push_back(ownedTerritories[weekestArmy]->getTerritoryArmy()->getArmyNumber());
		
		if (riceStock >= totalRiceUpkeep)
		{
			std::cout << kingdomName << " paid army upkeep: " << totalRiceUpkeep << " Rice." << std::endl;
			resources.subFromStock(RICE, totalRiceUpkeep);
			break;
		}
		else
		{
			std::cout << kingdomName << " does not enough Rice to pay upkeep of army. ";
			std::cout << "Army at " << ownedTerritories[weekestArmy]->getTerritoryName() << " deserted." << std::endl;
			ownedTerritories[weekestArmy]->getTerritoryArmy()->desertArmy();
		}
	}
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

			if (ownedTerritories[randomTerritory]->getTerritoryBuilding() != nullptr) buildingCount[ownedTerritories[randomTerritory]->getTerritoryBuilding()->getBuildingType()]--;
			ownedTerritories[randomTerritory]->destroyAProperty();
			ownedTerritories[randomTerritory]->setTerritoryRuler(NEUTRAL);
			ownedTerritories.erase(ownedTerritories.begin() + randomTerritory);
		}
		stability = 30;
	}
}
