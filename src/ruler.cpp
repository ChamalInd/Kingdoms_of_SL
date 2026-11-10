#include "../includes/ruler.h"

void Ruler::printRoundSummary()
{
	std::cout << kingdom.getKingdomName() << " (" << rulerName << ")" << std::endl;
	std::vector<int> resources = kingdom.getResources();

	std::cout << "Gold: " << resources[3] << " | Rice: " << resources[0] << " | Timber: " << resources[1] << " | Iron: " << resources[2] << std::endl;
	std::cout << "Population: " << kingdom.getPopulation() << " | Stability: " << kingdom.getStability() << std::endl;
	std::cout << "Territories: " << kingdom.getNoOfTerritoriesOwned() << " | Buildings: " << "0" << " | Units: " << "0" << std::endl;
}

int Ruler::calculateScore()
{
	std::vector<int> resources = kingdom.getResources();
	score = (20 * kingdom.getNoOfTerritoriesOwned()) + (5 * 0) + (3 * 0) + (resources[3] / 50) + (kingdom.getPopulation() / 100);
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

int Ruler::calculateTheTerritoryClaimCost()
{
	return 100 + 25 * kingdom.getNoOfTerritoriesOwned();
}

void Ruler::claimTerritory(Territory* territory)
{
	int cost = calculateTheTerritoryClaimCost();
	std::string terrainTypes[] = {"Paddy Plain", "Hill Country", "Dry Zone", "Coast", "Jungle", "Lake"};
	std::vector<int> resourceIncrement = {0, 0, 0, 0 - cost};
	kingdom.getResourceObject() += resourceIncrement;
	territory->setTerritoryRuler(rulerEnum);
	kingdom.claimTerritory(territory);

	std::cout << kingdom.getKingdomName() << " claimed " << territory->getTerritoryName() << " (";
	std::cout << terrainTypes[territory->getTerritoryType()] << ") for " << cost << " Gold." << std::endl;
}

void Ruler::claimPhase(WorldMap& worldMap)
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	if (potentialTerritories.empty() || claimBudget() < calculateTheTerritoryClaimCost())
	{
		return;
	}

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

	claimTerritory(potentialClaims[rand() % potentialClaims.size()]);
}

void Ruler::playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails) 
{
	claimPhase(worldMap);
	
}
