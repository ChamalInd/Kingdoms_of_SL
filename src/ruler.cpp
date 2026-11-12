#include "../includes/ruler.h"

void Ruler::printRoundSummary()
{
	std::cout << kingdom.getKingdomName() << " (" << rulerName << ")" << std::endl;
	Resources resources = kingdom.getResourceObject();

	std::cout << "Gold: " << resources.getGoldStock() << " | Rice: " << resources.getRiceStock();
	std::cout << " | Timber: " << resources.getTimberStock() << " | Iron: " << resources.getIronStock() << std::endl;
	std::cout << "Population: " << kingdom.getPopulation() << " | Stability: " << kingdom.getStability() << std::endl;
	std::cout << "Territories: " << kingdom.getNoOfTerritoriesOwned() << " | Buildings: " << kingdom.getNoOfBuildingsBuilt() << " | Units: " << "0" << std::endl;
}

int Ruler::calculateScore()
{
	int goldStock = kingdom.getResourceObject().getGoldStock();
	score = (20 * kingdom.getNoOfTerritoriesOwned()) + (5 * 0) + (3 * 0) + (goldStock / 50) + (kingdom.getPopulation() / 100);
	return score;
}

std::vector<Territory*> Ruler::checkNeighbouringTerritory(WorldMap& worldMap)
{
	std::vector<Territory*> potentialTerritories;
	std::vector<Territory*> ownedTerritories = kingdom.getOwnedTerritories();	

	for (auto& T : ownedTerritories)
	{
		std::vector<std::pair<int, int>> territories = T->getTerritoryNeighbours();
		for (auto& P : territories)
		{
			Territory* territory = worldMap.getTerritory(P.first, P.second);
			auto found = std::find(potentialTerritories.begin(), potentialTerritories.end(), territory);
			if (found == potentialTerritories.end() && territory->getTerritoryRuler() == NEUTRAL)
			{
				potentialTerritories.push_back(territory);
			}
		}
	}

	return potentialTerritories;
}

void Ruler::claimTerritory(Territory* territory, int claimCost)
{
	std::string terrainTypes[] = {"Paddy Plain", "Hill Country", "Dry Zone", "Coast", "Jungle", "Lake"};
	kingdom.getResourceObject().subFromStock(3, claimCost);
	territory->setTerritoryRuler(rulerEnum);
	kingdom.claimTerritory(territory);

	std::cout << std::endl << kingdom.getKingdomName() << " claimed " << territory->getTerritoryName() << " (";
	std::cout << terrainTypes[territory->getTerritoryType()] << ") for " << claimCost << " Gold." << std::endl;

}

void Ruler::constructBuilding(Territory* territory, std::unique_ptr<Building>& building)
{
	territory->buildAProperty(building);
	Resources buildingCost = territory->getTerritoryBuilding()->getBuildingCost();
	kingdom.getResourceObject() -= buildingCost;
	kingdom.increaseSpecificBuildingCount(territory->getTerritoryBuilding()->getBuildingType());

	if (territory->getTerritoryBuilding()->getBuildingType() == BARRACKS)
	{
		kingdom.increaseArmyCount();
		territory->addAnArmy(std::make_unique<Army>(kingdom.getArmyCount()));
	}

	std::cout << std::endl << kingdom.getKingdomName() << " built a " << territory->getTerritoryBuilding()->getBuildingName() << " in " << territory->getTerritoryName() << ". Cost: ";
	if (buildingCost.getGoldStock() > 0) std::cout << buildingCost.getGoldStock() << " Gold";
	if (buildingCost.getTimberStock() > 0) std::cout << ", " << buildingCost.getTimberStock() << " Timber";
	if (buildingCost.getIronStock() > 0) std::cout << ", " << buildingCost.getIronStock() << " Iron";
	std::cout << "." << std::endl;
}

void Ruler::recruiteUnits(Territory* territory, std::vector<std::unique_ptr<Unit>>& units, Resources totalRecruitementCost)
{
	std::vector<std::string> unitNames = {"Spearmen", "Archers", "Cavalries", "War Elephants"};
	std::vector<int> unitCount = {0, 0, 0, 0};

	for (auto& u : units)
	{
		unitCount[u->getUnitType()] += 1;
		territory->getTerritoryArmy()->recruiteUnits(u);
	}

	kingdom.getResourceObject() -= totalRecruitementCost;

	std::cout << kingdom.getKingdomName() << " recruited ";
	for (int i = 0; i < 4; i++) if (unitCount[i] > 0) std::cout << unitCount[i] << " " << unitNames[i] << " ";
	std::cout << " at " << territory->getTerritoryName() << ". Cost: " << std::endl;
	std::cout << totalRecruitementCost[3] << " Gold, " << totalRecruitementCost[2] << " Iron." << std::endl;
}

void Ruler::claimPhase(WorldMap& worldMap)
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	int claimCost = 100 + 25 * kingdom.getNoOfTerritoriesOwned();
	if (potentialTerritories.empty() || claimBudget() < claimCost) return;

	int highestScore = scoreTerritory(potentialTerritories[0]);
	std::vector<Territory*> potentialClaims;
	for (Territory* T : potentialTerritories)
	{
		int s = scoreTerritory(T);
		if (s > highestScore) 
		{
			highestScore = s;
			potentialClaims.clear();
		}
		if (s == highestScore) potentialClaims.push_back(T);
	}

	Territory* territory = potentialClaims[rand() % potentialClaims.size()];
	claimTerritory(territory, claimCost);
}

void Ruler::buildPhase()
{
	std::vector<Territory*> potentialTerritories;

	for (Territory* T : kingdom.getOwnedTerritories())
	{
		if (T->getTerritoryBuilding() == nullptr)
		{
			potentialTerritories.push_back(T);
		}
	}

	// no territories left to build
	if (potentialTerritories.empty()) return;

	std::vector<BuildingType> buildingTypes = {PADDY_FARM, IRRIGATION_LAKE, MINE, SAWMILL, MARKET, TEMPLE, BARRACKS, FORT};
	std::unique_ptr<Building> building;
	Territory* territory = nullptr;

	while (!buildingTypes.empty())
	{
		BuildingType selected = buildingTypes[0];
		int highestScore = scoreBuilding(selected);
		int index = 0;
		for (size_t i = 1; i < buildingTypes.size(); i++)
		{
			int s = scoreBuilding(buildingTypes[i]);
			if (highestScore < s) 
			{
				highestScore = s;
				selected = buildingTypes[i];
				index = i;
			}
		}

		building = Builder::createBuilding(selected);
		if (building == nullptr)
		{
			buildingTypes.erase(buildingTypes.begin() + index);
			continue;
		}
		std::vector<TerrainType> allowedTerrains = building->getAllowedTerrains();
		Resources buildingCost = building->getBuildingCost();

		if (!(kingdom.getResourceObject() >= buildingCost))
		{
			buildingTypes.erase(buildingTypes.begin() + index);
			continue;
		}

		bool found = false;
		for (Territory* T : potentialTerritories)
		{
			auto s = std::find(allowedTerrains.begin(), allowedTerrains.end(), T->getTerritoryType());
			if (s != allowedTerrains.end())
			{
				found = true;
				territory = T;
				break;
			}
		}

		if (found) break;
		else buildingTypes.erase(buildingTypes.begin() + index);
	}

	if (territory == nullptr) return;
	constructBuilding(territory, building);
}


void Ruler::recruitePhase()
{
	if (kingdom.getArmyCount() == 0) return;
	std::vector<Territory*> ownedTerritories = kingdom.getOwnedTerritories();
	std::vector<int> recruitementBudget = kingdom.getResources();
	Resources totalRecruitementCost = Resources({0, 0, 0, 0});
	int unitCount = INT_MAX;
	int armyIndex = -1;

	for (size_t i = 0; i < ownedTerritories.size(); i++)
	{
		if (ownedTerritories[i]->getTerritoryBuilding() == nullptr) continue;
		if (ownedTerritories[i]->getTerritoryBuilding()->getBuildingType() != BARRACKS) continue;
		if (ownedTerritories[i]->getTerritoryBuilding()->getBuildingStatus() == CLOSE) continue;
		if (ownedTerritories[i]->getTerritoryArmy()->getUnitCount() < unitCount)
		{
			unitCount = ownedTerritories[i]->getTerritoryArmy()->getUnitCount(); 
			armyIndex = i;
		}
	}

	if (armyIndex == -1) return;
	
	Territory* selectedTerritory = ownedTerritories[armyIndex];

	std::vector<UnitType> unitTypes = {SPEARMAN, ARCHER, CAVALRY, WAR_ELEPHANT};

	UnitType selectedUnit = unitTypes[0];
	int unitScore = scoreUnits(selectedUnit);
	std::vector<std::unique_ptr<Unit>> selectedUnits;

	while (!unitTypes.empty())
	{
		int unitIndex = 0;
		std::unique_ptr<Unit> unit = nullptr;
		for (size_t i = 0; i < unitTypes.size(); i++)
		{
			int score = scoreUnits(unitTypes[i]);
			if (score > unitScore)
			{
				unitScore = score;
				selectedUnit = unitTypes[i];
				unitIndex = i;
			}
		}
		
		if (unitScore == -1)
		{
			unitTypes.erase(unitTypes.begin() + unitIndex);
			continue;
		}
		unit = selectedTerritory->getTerritoryBuilding()->createUnit(selectedUnit);
		if (unit == nullptr) 
		{
			unitTypes.erase(unitTypes.begin() + unitIndex);
			continue;
		}
		
		Resources unitCost = unit->getUnitCost();
		totalRecruitementCost += unitCost;

		if (!(kingdom.getResourceObject() >= totalRecruitementCost))
		{
			totalRecruitementCost -= unitCost;
			unitTypes.erase(unitTypes.begin() + unitIndex);
			continue;
		}
		selectedUnits.push_back(std::move(unit));

		if (selectedUnits.size() == 5) break;
	}

	if (selectedUnits.empty()) return;

	recruiteUnits(selectedTerritory, selectedUnits, totalRecruitementCost);
}

void Ruler::playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails) 
{
	claimPhase(worldMap);
	buildPhase();
	recruitePhase();
}
