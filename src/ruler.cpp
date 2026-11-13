#include "ruler.h"

void Ruler::printRoundSummary()
{
	std::cout << kingdom->getKingdomName() << " (" << rulerName << ")" << std::endl;
	Resources resources = kingdom->getResourceObject();

	std::cout << "Gold: " << resources.getGoldStock() << " | Rice: " << resources.getRiceStock();
	std::cout << " | Timber: " << resources.getTimberStock() << " | Iron: " << resources.getIronStock() << std::endl;
	std::cout << "Population: " << kingdom->getPopulation() << " | Stability: " << kingdom->getStability() << std::endl;
	std::cout << "Territories: " << kingdom->getNoOfTerritoriesOwned() << " | Buildings: " << kingdom->getNoOfBuildingsBuilt() << " | Units: " << kingdom->getUnitCount() << std::endl;
}

int Ruler::calculateScore()
{
	int goldStock = kingdom->getResourceObject().getGoldStock();
	score = (20 * kingdom->getNoOfTerritoriesOwned()) + (5 * kingdom->getNoOfBuildingsBuilt()) + (3 * kingdom->getUnitCount()) + (goldStock / 50) + (kingdom->getPopulation() / 100);
	return score;
}

int Conqueror::scoreBuilding(const BuildingType type) const 
{
	if (type == BARRACKS && kingdom->getNoOfTerritoriesOwned() % 2 == 0 && kingdom->getSpecificBuildingCount(type) < 4) return 10;
	else if (type == TEMPLE) return -1;
	else return 5;
}
		
int Conqueror::scoreUnits(const UnitType unit) const 
{
	if (unit == WAR_ELEPHANT) return 10;
	if (unit == CAVALRY) return 9;
	else return 0;
}

int Conqueror::claimBudget() const { return kingdom->getResources()[3] * 40 / 100; }

int AgrarianSage::scoreBuilding(const BuildingType type) const 
{
	if (type == PADDY_FARM) return 10; 
	else if (type == IRRIGATION_LAKE) return 9;
	else if (type == TEMPLE && kingdom->getNoOfTerritoriesOwned() % 4 == 0) return 8;
	else return 0;
}

int AgrarianSage::scoreUnits(const UnitType unit) const
{
	if (unit == SPEARMAN) return 10;
	else return -1;
}

int AgrarianSage::claimBudget() const { return kingdom->getResources()[3]; }

int AgrarianSage::scoreTerritory(const Territory* T) const { return T->getTerritoryType() == PADDY_PLAIN ? 10 : 0; }

int MerchantPrince::scoreBuilding(const BuildingType type) const
{
	if (type == MARKET) return 10; 
	else return 0;
}
		
int MerchantPrince::scoreUnits(const UnitType unit) const
{
	if (unit == ARCHER) return 10;
	else if (unit == WAR_ELEPHANT) return 9;
	else return -1;
}

int MerchantPrince::claimBudget() const { return kingdom->getResources()[3] - 1500; }
		
int MerchantPrince::scoreTerritory(const Territory* T) const { return T->getTerritoryType() == COAST ? 10 : 0; }

int Strategist::scoreBuilding(const BuildingType type) const 
{
	if (type == PADDY_FARM) return 10;
	else if (type == TEMPLE) return 9;
	else return 0;
}

int Strategist::scoreUnits(const UnitType unit) const
{
	if (unit == SPEARMAN) return 10;
	else return -1;
}

int Strategist::claimBudget() const { return kingdom->getResources()[3]; }

std::vector<Territory*> Ruler::checkNeighbouringTerritory(WorldMap& worldMap)
{
	std::vector<Territory*> potentialTerritories;
	std::vector<Territory*> ownedTerritories = kingdom->getOwnedTerritories();	

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

void Ruler::recruiteUnits(Territory* territory, std::vector<std::unique_ptr<Unit>>& units, Resources totalRecruitementCost)
{
	std::vector<std::string> unitNames = {"Spearmen", "Archers", "Cavalries", "War Elephants"};
	std::vector<int> unitCount = {0, 0, 0, 0};

	for (auto& u : units)
	{
		unitCount[u->getUnitType()] += 1;
		territory->getTerritoryArmy()->recruiteUnits(u);
	}

	kingdom->getResourceObject() -= totalRecruitementCost;

	std::cout << std::endl << kingdom->getKingdomName() << " recruited ";
	for (int i = 0; i < 4; i++) if (unitCount[i] > 0) std::cout << unitCount[i] << " " << unitNames[i] << " ";
	std::cout << " at " << territory->getTerritoryName() << ". Cost: ";
	std::cout << totalRecruitementCost.getGoldStock() << " Gold, " << totalRecruitementCost.getIronStock() << " Iron." << std::endl;
}

void Ruler::claimPhase(WorldMap& worldMap)
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	int claimCost = 100 + 25 * kingdom->getNoOfTerritoriesOwned();
	std::cout << "visited";
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
	std::cout << "ready to purchase";

	Territory* territory = potentialClaims[rand() % potentialClaims.size()];
	kingdom->claimTerritory(territory, claimCost);
}

void Ruler::buildPhase()
{
	// no territories left to build
	if (kingdom->getNoOfBuildingsBuilt() == kingdom->getNoOfTerritoriesOwned()) return;

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

		if (highestScore < 0)
		{
			buildingTypes.erase(buildingTypes.begin() + index);
			continue;
		}

		building = BuildingFactory::createBuilding(selected);
		if (building == nullptr)
		{
			buildingTypes.erase(buildingTypes.begin() + index);
			continue;
		}
		Resources buildingCost = building->getBuildingCost();

		if (!(kingdom->getResourceObject() >= buildingCost))
		{
			buildingTypes.erase(buildingTypes.begin() + index);
			continue;
		}

		bool found = false;
		for (Territory* T : kingdom->getOwnedTerritories())
		{
			if (T->getTerritoryBuilding() != nullptr) continue;
			if (building->getIsAllowed(T->getTerritoryType()))
			{
				found = true;
				territory = T;
				break;
			}
		}

		if (found) break;
		buildingTypes.erase(buildingTypes.begin() + index);
	}

	if (territory == nullptr) return;
	kingdom->constructBuilding(territory, building);
}

void Ruler::recruitePhase()
{
	if (kingdom->getArmyCount() == 0) return;
	std::vector<Territory*> ownedTerritories = kingdom->getOwnedTerritories();
	std::vector<int> recruitementBudget = kingdom->getResources();
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
		unit = UnitFactory::createUnit(selectedUnit);
		if (unit == nullptr) 
		{
			unitTypes.erase(unitTypes.begin() + unitIndex);
			continue;
		}

		int currentArmyUpkeep = kingdom->getArmyUpkeep();
		Resources unitCost = unit->getUnitCost();
		totalRecruitementCost += unitCost;
		totalRecruitementCost.addToStock(RICE, currentArmyUpkeep + unit->getUnitUpkeep());

		if (!(kingdom->getResourceObject() >= totalRecruitementCost))
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
