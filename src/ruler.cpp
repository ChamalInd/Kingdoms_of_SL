#include "../includes/ruler.h"

void Ruler::printRoundSummary()
{
	std::cout << kingdom.getKingdomName() << " (" << rulerName << ")" << std::endl;
	std::vector<int> resources = kingdom.getResources();

	std::cout << "Gold: " << resources[0] << " | Rice: " << resources[1] << " | Timber: " << resources[2] << " | Iron: " << resources[3] << std::endl;
	std::cout << "Population: " << kingdom.getPopulation() << " | Stability: " << kingdom.getStability() << std::endl;
	std::cout << "Territories: " << kingdom.getNoOfTerritoriesOwned() << " | Buildings: " << "0" << " | Units: " << "0" << std::endl;
}

int Ruler::calculateScore()
{
	std::vector<int> resources = kingdom.getResources();
	score = (20 * kingdom.getNoOfTerritoriesOwned()) + (5 * 0) + (3 * 0) + (resources[0] / 50) + (kingdom.getPopulation() / 100);
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

void Conqueror::playTurn(WorldMap& worldMap) 
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
	std::cout << potentialTerritories[0]->getTerritoryName() << std::endl;
}

void AgrarianSage::playTurn(WorldMap& worldMap) 
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
}

void MerchantPrince::playTurn(WorldMap& worldMap) 
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
}

void Strategist::playTurn(WorldMap& worldMap) 
{
	std::vector<Territory*> potentialTerritories = checkNeighbouringTerritory(worldMap);	
}
