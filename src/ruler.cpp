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

// score territory functions
int AgrarianSage::scoreTerritory(const Territory* T) const { return T->getTerritoryType() == PADDY_PLAIN ? 10 : 0; }

int MerchantPrince::scoreTerritory(const Territory* T) const { return T->getTerritoryType() == COAST ? 10 : 0; }

// score building functions
int Conqueror::scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const 
{
	if (type == BARRACKS && kingdom->getNoOfTerritoriesOwned() % 2 == 0 && kingdom->getSpecificBuildingCount(type) < 4) return 10;
	if (type == TEMPLE) return -1;
	if (type == FORT) return 10;
	return 0;
}

int AgrarianSage::scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const 
{
	if (type == PADDY_FARM) return 10; 
	if (type == IRRIGATION_LAKE) return 10;
	if (type == TEMPLE && kingdom->getNoOfTerritoriesOwned() % 4 == 0) return 10;
	return 0;
}

int MerchantPrince::scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const
{
	if (type == MARKET) return 10; 
	return 0;
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

// score unit functions
int Conqueror::scoreUnits(const UnitType unit, std::vector<RulerDetails>) const 
{
	if (unit == WAR_ELEPHANT) return 10;
	if (unit == CAVALRY) return 10;
	return 0;
}

int AgrarianSage::scoreUnits(const UnitType unit, std::vector<RulerDetails>) const
{
	if (kingdomState == AT_WAR && unit == SPEARMAN) return 10;
	return -1;
}

int MerchantPrince::scoreUnits(const UnitType unit, std::vector<RulerDetails>) const
{
	if (unit == ARCHER) return 10;
	if (unit == WAR_ELEPHANT) return 10;
	return -1;
}

int Strategist::scoreUnits(const UnitType unit, std::vector<RulerDetails> rd) const
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

	if (defensive && unit == SPEARMAN) return 10;
	return -1;
}

// check for move permissions
bool Conqueror::hasPermissionToMove(Territory* territory, std::vector<RulerDetails> rulerDetails) const
{
	if (territory->getTerritoryRuler() == NEUTRAL || territory->getTerritoryRuler() == rulerEnum) return true;	
	int myStrength = 0;
	int opponentStrength = 0;
	for (RulerDetails R : rulerDetails)
	{
		if (R.ruler == rulerEnum) myStrength = R.armyStrength;
		if (R.ruler != territory->getTerritoryRuler()) continue;
		opponentStrength = R.armyStrength;
		break;
	}
	if (myStrength >= opponentStrength * 120 / 100) return true;
	return false;
}

bool AgrarianSage::hasPermissionToMove(Territory* territory, std::vector<RulerDetails>) const
{
	if (territory->getTerritoryRuler() == NEUTRAL || territory->getTerritoryRuler() == rulerEnum) return true;	
	return false;
}

bool MerchantPrince::hasPermissionToMove(Territory* territory, std::vector<RulerDetails> rulerDetails) const
{
	if (territory->getTerritoryRuler() == NEUTRAL || territory->getTerritoryRuler() == rulerEnum) return true;	
	int myStrength = 0;
	int opponentStrength = 0;
	for (RulerDetails R : rulerDetails)
	{
		if (R.ruler == rulerEnum) myStrength = R.armyStrength;
		if (R.ruler != territory->getTerritoryRuler()) continue;
		opponentStrength = R.armyStrength;
		break;
	}
	if (myStrength >= opponentStrength * 2) return true;

	return false;
}

bool Strategist::hasPermissionToMove(Territory* territory, std::vector<RulerDetails> rulerDetails) const
{
	if (territory->getTerritoryRuler() == NEUTRAL || territory->getTerritoryRuler() == rulerEnum) return true;	
	int myTerritoryCount = 0;
	int opponentTerritoryCount = 0;
	for (RulerDetails R : rulerDetails)
	{
		if (R.ruler == rulerEnum) myTerritoryCount = R.noOfTerritoriesOwn;
		if (R.ruler != territory->getTerritoryRuler()) continue;
		opponentTerritoryCount = R.noOfTerritoriesOwn;
		break;
	}
	if (myTerritoryCount >= opponentTerritoryCount + 2 && kingdom->getStability() >= 60) return true;
	return false;
}

// territory claim budget
int Conqueror::claimBudget() const { return kingdom->getResources()[GOLD] * 40 / 100; }

int AgrarianSage::claimBudget() const { return kingdom->getResources()[GOLD]; }

int MerchantPrince::claimBudget() const { return std::max(0, kingdom->getResources()[GOLD] - 1500); }

int Strategist::claimBudget() const { return kingdom->getResources()[GOLD]; }

// construction budget
Resources Conqueror::constructionBudget(std::unique_ptr<Event>&) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock() * 40 / 100});
	return budget;
}
		
Resources AgrarianSage::constructionBudget(std::unique_ptr<Event>&) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	return budget;
}

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

Resources Strategist::constructionBudget(std::unique_ptr<Event>&) const
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock()});
	return budget;
}
		
// recruite budget
Resources Conqueror::recruiteBudget() const 
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			kingdom->getResourceObject().getGoldStock() * 60 / 100});
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
		
Resources MerchantPrince::recruiteBudget() const 
{
	Resources budget = Resources({
			kingdom->getResourceObject().getRiceStock(),
			kingdom->getResourceObject().getTimberStock(), 
			kingdom->getResourceObject().getIronStock(), 
			std::max(0, kingdom->getResourceObject().getGoldStock() - 1500)});
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
		
void Ruler::initiateWar(Ruler* attacker, Ruler* defender, Territory* territory, int attackArmy)
{
	std::string terrainTypes[] = {"Paddy Plain", "Hill Country", "Dry Zone", "Coast", "Jungle", "Lake"};
	std::cout << std::endl;
	Logger::printTitle(" Ongoing War ", '*');
	std::cout << std::endl << "Battle at " << territory->getTerritoryName() << " (" << terrainTypes[territory->getTerritoryType()] << ")!" << std::endl;
	std::cout << "Attacker: " << attacker->getRulerName() << " (Strength: " << attacker->getKingdom()->getArmyStrength(0) << ")" << std::endl;
	std::cout << "Defender: " << defender->getRulerName() << " (Strength: " << defender->getKingdom()->getArmyStrength(0) << ")" << std::endl;

	int defendArmy = -1;
	for (size_t i = 0; i < defender->getKingdom()->getOwnedArmies().size(); i++)
	{
		if (defender->getKingdom()->getOwnedArmies()[i]->getStationedPlace() != territory) continue;
		defendArmy = i;
		break;
	}

	if (defendArmy == -1) endWar(attacker, defender, territory);
}

void Ruler::endWar(Ruler* winner, Ruler* loser, Territory* territory)
{
	std::cout << std::endl << loser->getRulerName() << "'s army has been destroyed." << std::endl;
	std::cout << winner->getRulerName() << " captured " << territory->getTerritoryName() << "." << std::endl;
	
	auto loserTerritories = loser->getKingdom()->getOwnedTerritories();
	auto found = std::find(loserTerritories.begin(), loserTerritories.end(), territory);
	if (found == loserTerritories.end()) return;
	int index = std::distance(loserTerritories.begin(), found);

	loser->getKingdom()->abandonTerritory(index);
	winner->getKingdom()->claimTerritory(territory, 0);
}

void Ruler::claimPhase(WorldMap& worldMap, Territory* territory)
{
	int claimCost = 100 + 25 * kingdom->getNoOfTerritoriesOwned();
	if (claimBudget() < claimCost) return;

	if (territory == nullptr)
	{
		std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
		if (potentialTerritories.empty()) return;

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

		territory = potentialClaims[rand() % potentialClaims.size()];
	}

	if (territory == nullptr) return;

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

void Ruler::recruitePhase(std::vector<RulerDetails> rd)
{
	if (kingdom->getArmyCount() == 0) return;
	std::vector<Territory*> potentialTerritories;
	Resources totalRecruitementCost = Resources({0, 0, 0, 0});
	int recruiteAmount = 0;

	for (auto& T : kingdom->getOwnedTerritories())
	{
		if (T->getTerritoryBuilding() == nullptr) continue;
		if (T->getTerritoryBuilding()->getBuildingType() != BARRACKS) continue;
		if (T->getTerritoryBuilding()->getBuildingStatus() == CLOSE) continue;
		potentialTerritories.push_back(T);
	}

	if (potentialTerritories.empty()) return;
	
	Territory* selectedTerritory = potentialTerritories[rand() % potentialTerritories.size()];
	if (selectedTerritory == nullptr) return;

	std::vector<UnitType> unitTypes = {SPEARMAN, ARCHER, CAVALRY, WAR_ELEPHANT};

	UnitType selected;
	int unitScore;
	std::unique_ptr<Unit> selectedUnit = nullptr;
	int unitIndex = 0;
	
	while (!unitTypes.empty())
	{
		if (selectedUnit == nullptr)
		{
			unitIndex = 0;
			selected = unitTypes[0];
			unitScore = scoreUnits(selected, rd);
			for (size_t i = 0; i < unitTypes.size(); i++)
			{
				int score = scoreUnits(unitTypes[i], rd);
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
		if (recruiteBudget().getRiceStock() < (currentArmyUpkeep + recruiteAmount * selectedUnit->getUnitUpkeep()))
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
	
void Ruler::movePhase(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails, std::vector<std::unique_ptr<Ruler>>& rulers)
{
	std::vector<int> triedArmies;
	int index = -1;
	while (triedArmies.size() < kingdom->getArmyCount())
	{
		index = rand() % kingdom->getArmyCount(); 
		auto found = std::find(triedArmies.begin(), triedArmies.end(), index);
		if (found != triedArmies.end()) 
		{
			index = 0;
			continue;
		}
		triedArmies.push_back(index);
		if (kingdom->getOwnedArmies()[index]->getUnitCount() <= 0) 
		{
			index = 0;
			continue;
		}
		break;
	}

	if (index == -1) return;

	int moveLength = kingdom->getOwnedArmies()[index]->getMove();
	Territory* selected = kingdom->getOwnedArmies()[index]->getStationedPlace();
	Territory* goingToMove = nullptr;
	std::vector<std::pair<int, int>> visited;
	while (moveLength > 0)
	{
		std::pair<int, int> location;
		if (goingToMove == nullptr) location = selected->getTerritoryNeighbours()[rand() % selected->getTerritoryNeighbours().size()];
		else location = goingToMove->getTerritoryNeighbours()[rand() % goingToMove->getTerritoryNeighbours().size()];
		
		auto found = std::find(visited.begin(), visited.end(), location);
		if (found != visited.end()) continue;
		visited.push_back(location);
		goingToMove = worldMap.getTerritory(location.first, location.second);
		moveLength--;
	}

	if (goingToMove == nullptr) return;


	if (hasPermissionToMove(goingToMove, rulerDetails))
	{
		std::cout << std::endl << kingdom->getKingdomName();
		kingdom->getOwnedArmies()[index]->moveArmy(goingToMove);
	
		if (goingToMove->getTerritoryRuler() == NEUTRAL) claimPhase(worldMap, goingToMove);

		if (goingToMove->getTerritoryRuler() != NEUTRAL && goingToMove->getTerritoryRuler() != rulerEnum)
		{
			Ruler* attacker;
			Ruler* defender;
			for (auto& R : rulers)
			{
				if (goingToMove->getTerritoryRuler() == R->getRulerEnum()) defender = R.get();
				if (rulerEnum == R->getRulerEnum()) attacker = R.get();
			}

			initiateWar(attacker, defender, goingToMove, index);
		}
	}
}

void Ruler::playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails, std::unique_ptr<Event>& currentEvent, std::vector<std::unique_ptr<Ruler>>& rulers) 
{
	claimPhase(worldMap, nullptr);
	buildPhase(currentEvent, rulerDetails);
	recruitePhase(rulerDetails);
	movePhase(worldMap, rulerDetails, rulers);
}
