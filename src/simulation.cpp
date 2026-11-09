#include "../includes/includes.h"
		
SimulationManager::SimulationManager()
{
	// creating kingdoms and their rulers
	const std::string rulerNames[] = {"The Conqueror", "The Agrarian Sage", "The Merchant Prince", "The Strategist"};
	const std::string kingdomNames[] = {"Anuradhapura", "Ruhuna", "Kotte", "Kandy"};
	const std::vector<std::pair<int, int>> kingdomCapitals = {std::make_pair(0, 0), std::make_pair(0, 5), std::make_pair(5, 0), std::make_pair(5, 5)};
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		Kingdom kingdom(kingdomNames[i]);
		Territory* capital = worldMap.getTerritory(kingdomCapitals[i].first, kingdomCapitals[i].second);
		kingdom.setCapitalCity(capital);
		Ruler ruler(rulerNames[i], kingdom);
		rulers.push_back(ruler);
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
	
	determinePlayerOrder(rulers);
	std::sort(rulers.begin(), rulers.end(), [](const Ruler& a, const Ruler& b) {
		return a.getFirstDieValue() > b.getFirstDieValue();
	});
	std::cout << "Turn order : ";
	int count = 0;
	for (Ruler R : rulers)
	{
		std::cout << R.getKingdom().getKingdomName();
		count++;
		if (count < NO_OF_KINGDOMS) std::cout << ", ";
	}
	std::cout << "." << std::endl;

	worldMap.printWorldMap();
}

void SimulationManager::gameLoop()
{
	while (gameRound < 200)
	{
		for (Ruler& R : rulers)
		{
			Kingdom& kingdom = R.getKingdom();
			std::string title = " Round " + std::to_string(gameRound) + " : " + kingdom.getKingdomName() + " ";
			int lPad = (29 - title.length()) / 2;
			int rPad = 29 - title.length() - lPad;
			std::cout << std::string(20, '-') << std::string(lPad, '-') << title << std::string(rPad, '-') << std::string(20, '-') << std::endl;
			// collect productions
			kingdom.collectProductions();

		}
		gameRound++;
	}
}
