#include "../includes/includes.h"
		
SimulationManager::SimulationManager()
{
	// creating kingdoms
	const std::string kingdomNames[] = {"Anuradhapura", "Ruhuna", "Kotte", "Kandy"};
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		Kingdom kingdom(kingdomNames[i]);
		kingdoms.push_back(kingdom);
	}

	initializeGame();
}

void SimulationManager::initializeGame()
{
	std::cout << std::string(20, '=') << " KINGDOM OF LANKA Simulation " << std::string(20, '=') << std::endl << std::endl;
    std::cout << "Seed : " << SEED << std::endl << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 1 : Anuradhapura" << " - The Conqueror" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 2 : Ruhuna" << " - The Agrarian Sage" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 3 : Kotte" << " - The Merchant Prince" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 4 : Kandy" << " - The Strategist" << std::endl;

	std::cout << std::endl << "Each kingdom begins with 1000 Gold, 500 Rice, 200 Timber, 100 Iron." << std::endl;
	
	determinePlayerOrder(kingdoms);
	std::sort(kingdoms.begin(), kingdoms.end(), [](const Kingdom& a, const Kingdom& b) {
		return a.getFirstDieValue() > b.getFirstDieValue();
	});
	std::cout << "Turn order : ";
	int count = 0;
	for (Kingdom K : kingdoms)
	{
		std::cout << K.getKingdomName();
		count++;
		if (count < NO_OF_KINGDOMS) std::cout << ", ";
	}
	std::cout << "." << std::endl;
}
