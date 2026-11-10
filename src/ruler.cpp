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


void Conqueror::playTurn(WorldMap& worldMap) 
{

}

void AgrarianSage::playTurn(WorldMap& worldMap) 
{

}

void MerchantPrince::playTurn(WorldMap& worldMap) 
{

}

void Strategist::playTurn(WorldMap& worldMap) 
{

}
