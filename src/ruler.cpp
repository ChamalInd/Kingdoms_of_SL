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

void Conqueror::playTurn(WorldMap& worldMap) 
{
	std::vector<int> currentResources = kingdom.getResources();
	// claiming territories
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	if (!potentialTerritories.empty() && (currentResources[3] * 40 / 100) >= calculateTheTerritoryClaimCost()) // saves 60% of gold for military purpose
	{
		int number = rand() % potentialTerritories.size();
		claimTerritory(potentialTerritories[number]);
	}
}

void AgrarianSage::playTurn(WorldMap& worldMap) 
{
	std::vector<int> currentResources = kingdom.getResources();
	// claiming territories
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	if (!potentialTerritories.empty() && currentResources[3] >= calculateTheTerritoryClaimCost())
	{
		bool claimed = false;
		for (Territory* T : potentialTerritories)
		{
			if (T->getTerritoryType() == PADDY_PLAIN)
			{
				claimTerritory(T);
				claimed = true;
				break;
			}
		}
		if (!claimed)
		{
			int number = rand() % potentialTerritories.size();
			claimTerritory(potentialTerritories[number]);
		}
	}
}

void MerchantPrince::playTurn(WorldMap& worldMap) 
{
	std::vector<int> currentResources = kingdom.getResources();
	// claiming territories
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	if (!potentialTerritories.empty() && (currentResources[3] - 1500) >= calculateTheTerritoryClaimCost())
	{
		int number = rand() % potentialTerritories.size();
		claimTerritory(potentialTerritories[number]);
	}

}

void Strategist::playTurn(WorldMap& worldMap) 
{
	std::vector<int> currentResources = kingdom.getResources();
	// claiming territories
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	if (!potentialTerritories.empty() && currentResources[3] >= calculateTheTerritoryClaimCost())
	{
		int number = rand() % potentialTerritories.size();
		claimTerritory(potentialTerritories[number]);
	}

}
