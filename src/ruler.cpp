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

int Conqueror::scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const 
{
	if (type == BARRACKS && kingdom->getNoOfTerritoriesOwned() % 2 == 0 && kingdom->getSpecificBuildingCount(type) < 4) return 10;
	if (type == TEMPLE) return -1;
	if (type == FORT) return 10;
	return 0;
}
		
int Conqueror::scoreUnits(const UnitType unit) const 
{
	if (unit == WAR_ELEPHANT) return 10;
	if (unit == CAVALRY) return 9;
	return 0;
}

int Conqueror::claimBudget() const { return kingdom->getResources()[GOLD] * 40 / 100; }

Resources Conqueror::constructionBudget(std::unique_ptr<Event>&) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock() * 40 / 100});
	return budget;
}
		
Resources Conqueror::recruiteBudget() const 
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock() * 60 / 100});
	return budget;
}

int AgrarianSage::scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const 
{
	if (type == PADDY_FARM) return 10; 
	if (type == IRRIGATION_LAKE) return 10;
	if (type == TEMPLE && kingdom->getNoOfTerritoriesOwned() % 4 == 0) return 10;
	return 0;
}

int AgrarianSage::scoreUnits(const UnitType unit) const
{
	if (unit == SPEARMAN) return 10;
	return -1;
}

int AgrarianSage::claimBudget() const { return kingdom->getResources()[GOLD]; }

int AgrarianSage::scoreTerritory(const Territory* T) const { return T->getTerritoryType() == PADDY_PLAIN ? 10 : 0; }

Resources AgrarianSage::constructionBudget(std::unique_ptr<Event>&) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	return budget;
}
		
Resources AgrarianSage::recruiteBudget() const 
{
	int consumptionSavings = 5 * (kingdom->getPopulation() / 20);
	Resources budget = Resources({
			std::max(0, kingdom->getResourceObject().getRiceStock() - consumptionSavings),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	return budget;
}

int MerchantPrince::scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const
{
	if (type == MARKET) return 10; 
	return 0;
}
		
int MerchantPrince::scoreUnits(const UnitType unit) const
{
	if (unit == ARCHER) return 10;
	if (unit == WAR_ELEPHANT) return 9;
	return -1;
}

int MerchantPrince::claimBudget() const { return std::max(0, kingdom->getResources()[GOLD] - 1500); }
		
int MerchantPrince::scoreTerritory(const Territory* T) const { return T->getTerritoryType() == COAST ? 10 : 0; }

Resources MerchantPrince::constructionBudget(std::unique_ptr<Event>& currentEvent) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	if (currentEvent != nullptr && currentEvent->getEventType() == TRADE_CARAVAN) return budget;
	budget.subFromStock(GOLD, 1500);
	return budget;
}
		
Resources MerchantPrince::recruiteBudget() const 
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			std::max(0, kingdom->getResourceObject().getGoldStock() - 1500)});
	return budget;
}

int Strategist::scoreBuilding(const BuildingType type, std::vector<RulerDetails> rd, std::unique_ptr<Event>& currentEvent) const 
{
	bool defensive = false;
	int rulerArmyStrength = 0;
	int strongest = INT_MIN;
	for (RulerDetails R : rd)
	{
		if (R.ruler == rulerEnum) rulerArmyStrength = R.armyStrength;
		if (R.armyStrength > strongest) strongest = R.armyStrength;
	}

	if (rulerArmyStrength != strongest) defensive = true; 

	if (!defensive && type == PADDY_FARM) return 10;
	if (!defensive && type == TEMPLE) return 10;
	if (defensive && type == BARRACKS) return 10;
	if (defensive && type == FORT) return 10;
	if (currentEvent != nullptr && currentEvent->getEventType() == DROUGHT && type == PADDY_FARM) return 10;
	if (currentEvent != nullptr && currentEvent->getEventType() == PLAGUE && type == TEMPLE) return 10;
	else return 0;
}

int Strategist::scoreUnits(const UnitType unit) const
{
	if (unit == SPEARMAN) return 10;
	return -1;
}

int Strategist::claimBudget() const { return kingdom->getResources()[GOLD]; }

Resources Strategist::constructionBudget(std::unique_ptr<Event>&) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	return budget;
}
		
Resources Strategist::recruiteBudget() const 
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	return budget;
}

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

void Ruler::claimPhase(WorldMap& worldMap)
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	int claimCost = 100 + 25 * kingdom->getNoOfTerritoriesOwned();
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
	kingdom->claimTerritory(territory, claimCost);
}

void Ruler::buildPhase(std::unique_ptr<Event>& currentEvent, std::vector<RulerDetails> rd)
{
	// no territories left to build
	if (kingdom->getNoOfBuildingsBuilt() == kingdom->getNoOfTerritoriesOwned()) return;

	std::vector<BuildingType> buildingTypes = {PADDY_FARM, IRRIGATION_LAKE, MINE, SAWMILL, MARKET, TEMPLE, BARRACKS, FORT};
	std::unique_ptr<Building> building;
	Territory* territory = nullptr;

	while (!buildingTypes.empty())
	{
		BuildingType selected = buildingTypes[0];
		int highestScore = scoreBuilding(selected, rd, currentEvent);
		int index = 0;
		for (size_t i = 1; i < buildingTypes.size(); i++)
		{
			int s = scoreBuilding(buildingTypes[i], rd, currentEvent);
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

		if (!(constructionBudget(currentEvent) >= buildingCost))
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
	Resources totalRecruitementCost = Resources({0, 0, 0, 0});
	int unitCount = INT_MAX;
	int armyIndex = -1;
	int recruiteAmount = 0;

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
	if (selectedTerritory == nullptr) return;

	std::vector<UnitType> unitTypes = {SPEARMAN, ARCHER, CAVALRY, WAR_ELEPHANT};

	UnitType selected = unitTypes[0];
	int unitScore = scoreUnits(selected);
	std::unique_ptr<Unit> selectedUnit = nullptr;
	int unitIndex = 0;
	
	while (!unitTypes.empty())
	{
		if (selectedUnit == nullptr)
		{
			unitIndex = 0;
			for (size_t i = 0; i < unitTypes.size(); i++)
			{
				int score = scoreUnits(unitTypes[i]);
				if (score > unitScore)
				{
					unitScore = score;
					selected = unitTypes[i];
					unitIndex = i;
				}
			}
		
			if (unitScore == -1)
			{
				unitTypes.erase(unitTypes.begin() + unitIndex);
				selectedUnit = nullptr;
				continue;
			}
			selectedUnit = UnitFactory::createUnit(selected);
			if (selectedUnit == nullptr) 
			{
				unitTypes.erase(unitTypes.begin() + unitIndex);
				continue;
			}
		}
		
		int currentArmyUpkeep = kingdom->getArmyUpkeep();
		if (recruiteBudget().getRiceStock() < currentArmyUpkeep + recruiteAmount * selectedUnit->getUnitUpkeep())
		{
			selectedUnit = nullptr;
			unitTypes.erase(unitTypes.begin() + unitIndex);
			continue;
		}
		Resources unitCost = selectedUnit->getUnitCost();
		totalRecruitementCost += unitCost;

		if (!(recruiteBudget() >= totalRecruitementCost))
		{
			selectedUnit = nullptr;
			totalRecruitementCost -= unitCost;
			unitTypes.erase(unitTypes.begin() + unitIndex);
			continue;
		}
		recruiteAmount++;

		if (recruiteAmount == 5) break;
	}

	if (recruiteAmount == 0) return;
	
	kingdom->recruiteUnits(selectedTerritory, selected, recruiteAmount, totalRecruitementCost);
}

void Ruler::playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails, std::unique_ptr<Event>& currentEvent) 
{
	claimPhase(worldMap);
	buildPhase(currentEvent, rulerDetails);
	recruitePhase();
}
