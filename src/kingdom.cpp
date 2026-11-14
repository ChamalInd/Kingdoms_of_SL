#include "kingdom.h"

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

void Kingdom::claimTerritory(Territory* territory, int claimCost)
{
	if (claimCost != 0) 
	{
		std::string terrainTypes[] = {"Paddy Plain", "Hill Country", "Dry Zone", "Coast", "Jungle", "Lake"};
		resources.subFromStock(GOLD, claimCost);

		std::cout << std::endl << kingdomName << " claimed " << territory->getTerritoryName() << " (";
		std::cout << terrainTypes[territory->getTerritoryType()] << ") for " << claimCost << " Gold." << std::endl;
	}
	territory->setTerritoryRuler(rulerEnum);
	ownedTerritories.push_back(territory);
}
		
void Kingdom::abandonTerritory(int territoryIndex)
{
	destroyBuilding(ownedTerritories[territoryIndex]);
	ownedTerritories[territoryIndex]->setTerritoryRuler(NEUTRAL);
	ownedTerritories.erase(ownedTerritories.begin() + territoryIndex);
}

void Kingdom::constructBuilding(Territory* territory, std::unique_ptr<Building>& building)
{
	if (buildingCount[BARRACKS] > 0)
	{
		Resources buildingCost = building->getBuildingCost();
		resources  -= buildingCost;

		std::cout << std::endl << kingdomName << " built a " << building->getBuildingName() << " in " << territory->getTerritoryName() << ". Cost: ";
		if (buildingCost.getGoldStock() > 0) std::cout << buildingCost.getGoldStock() << " Gold";
		if (buildingCost.getTimberStock() > 0) std::cout << ", " << buildingCost.getTimberStock() << " Timber";
		if (buildingCost.getIronStock() > 0) std::cout << ", " << buildingCost.getIronStock() << " Iron";
		std::cout << "." << std::endl;
	}

	buildingCount[building->getBuildingType()]++;
	if (building->getBuildingType() == BARRACKS)
	{
		increaseArmyCount();
		territory->addAnArmy(std::make_unique<Army>(getArmyCount()));
	}
	territory->construct(building);
}

void Kingdom::destroyBuilding(Territory* territory)
{
	if (territory->getTerritoryBuilding() == nullptr) return;
	buildingCount[territory->getTerritoryBuilding()->getBuildingType()]--;
	territory->demolish();
}

void Kingdom::recruiteUnits(Territory* territory, UnitType unit, int amount, Resources totalRecruitementCost)
{
	for (int i = 0; i < amount; i++)
	{
		std::unique_ptr<Unit> newUnit = UnitFactory::createUnit(unit);
		territory->getTerritoryArmy()->recruiteUnits(newUnit);
	}

	resources -= totalRecruitementCost;
	
	std::vector<std::string> unitTypes = {"Spearman", "Archer", "Cavalry", "War Elephant"};
	std::cout << std::endl << kingdomName << " recruited " << amount << " " << unitTypes[unit] << " at " << territory->getTerritoryName();
	std::cout << ". Cost: " << totalRecruitementCost.getGoldStock() << " Gold, " << totalRecruitementCost.getIronStock() << " Iron." << std::endl;
}

void Kingdom::desertUnits()
{
	Territory* randomTerritory = nullptr;
	
	for (Territory* T : ownedTerritories)
	{
		if (T->getTerritoryArmy() == nullptr) continue;
		if (T->getTerritoryArmy()->getUnitCount() == 0) continue;
		randomTerritory = T;
		break;
	}

	randomTerritory->getTerritoryArmy()->desertUnit();
	std::cout << "Unit in " << randomTerritory->getTerritoryName() << " deserted." << std::endl;
}

void Kingdom::collectProductions(WorldMap& worldMap, RulerEnum ruler, std::unique_ptr<Event>& currentEvent)
{
	Resources resourceIncrements = Resources({0, 0, 0, 0});
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
		if (T->getTerritoryBuilding() == nullptr) continue;
		if (T->getTerritoryBuilding()->getBuildingType() == TEMPLE) continue;
		if (T->getTerritoryBuilding()->getBuildingStatus() == CLOSE) continue;
		
		Resources buildingResources = T->getTerritoryBuilding()->getResourceEffect(T, worldMap, ruler);
		resourceIncrements += buildingResources;
	}

	if (currentEvent != nullptr) 
	{
		ResourceEffect eventEffect = currentEvent->applyResourceEffect();
		resourceIncrements.scaleStock(eventEffect.resource, eventEffect.scale);
	}

	if (stability < 30) for (int i = 0; i < 4; i++) resourceIncrements.scaleStock(i, 75);
	
	resources += resourceIncrements;

	std::cout << kingdomName << " produced: Gold +" << resourceIncrements.getGoldStock() << ", Rice +" << resourceIncrements.getRiceStock();
	std::cout << ", Timber +" << resourceIncrements.getTimberStock() << ", Iron +" << resourceIncrements.getIronStock() << "." << std::endl << std::endl;
}
		
void Kingdom::populationActivities(std::unique_ptr<Event>& currentEvent)
{
	int riceConsumption = 0; 
	int taxIncome = 0;
	int currentRiceStock = resources.getRiceStock();	
	PopulationEffect eventEffect;
	eventEffect.populationScale = 100;
	eventEffect.consumptionScale = 100;

	if (currentEvent != nullptr) eventEffect = currentEvent->applyPopulationEffect();

	population = population * eventEffect.populationScale / 100;

	// consume rice
	int consumption = population / 20;
	consumption = consumption * eventEffect.consumptionScale / 100;

	if (consumption <= currentRiceStock) riceConsumption = consumption;

	if ((consumption * 2) <= currentRiceStock) 
	{
		population += population * 2 / 100;
		riceAbundance = MORE;
	}
	
	if (consumption > currentRiceStock)
	{
		riceConsumption = currentRiceStock;
		population -= population * 5 / 100;
		riceAbundance = LESS;
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

	int totalRiceUpkeep = 0;
		
	for (Territory* T : ownedTerritories)
	{
		if (T->getTerritoryArmy() == nullptr) continue;
		if (T->getTerritoryArmy()->getUnitCount() <= 0) continue;
		totalRiceUpkeep += T->getTerritoryArmy()->getUpkeep(); 
	}

	if (totalRiceUpkeep == 0) return;
		
	if (riceStock >= totalRiceUpkeep)
	{
		std::cout << kingdomName << " paid army upkeep: " << totalRiceUpkeep << " Rice." << std::endl;
	}
	else
	{
		std::cout << kingdomName << " does not enough Rice to pay upkeep of army. ";
		desertUnits();
		totalRiceUpkeep = riceStock;
	}
	resources.subFromStock(RICE, totalRiceUpkeep);
}

void Kingdom::updateStability(std::unique_ptr<Event>& currentEvent)
{
	int stabilityIncrement = 0;

	if (riceAbundance == MORE) stabilityIncrement += 1;
	else if (riceAbundance == LESS) stabilityIncrement -= 10;

	riceAbundance = ENOUGH;

	if (currentEvent != nullptr) stabilityIncrement += currentEvent->applyStabilityEffect();

	for (Territory* T : ownedTerritories)
	{
		if (T->getTerritoryBuilding() == nullptr) continue;
		if (T->getTerritoryBuilding()->getBuildingType() != TEMPLE) continue;
		stabilityIncrement += T->getTerritoryBuilding()->getStabilityEffect();
	}

	if (lastWarStatus == WON) stabilityIncrement += 2;
	else if (lastWarStatus == LOST) stabilityIncrement -= 5;

	lastWarStatus = NO_WAR;

	stability = std::clamp(stability + stabilityIncrement, 0, 100);

	if (stability <= 0)
	{
		if (getNoOfTerritoriesOwned() > 1)
		{
			int randomTerritory = rand() % (getNoOfTerritoriesOwned() - 1) + 1;
			std::cout << "Stability of " << kingdomName << " has reached 0. Rebellion in ";
			std::cout << ownedTerritories[randomTerritory]->getTerritoryName() << "! " << kingdomName << " loses control." << std::endl;

			abandonTerritory(randomTerritory);
		}
		stability = 30;
	}
}
