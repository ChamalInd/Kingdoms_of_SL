#include "../includes/simulation.h"
		
SimulationManager::SimulationManager()
{
	// creating kingdoms and their rulers
	const std::string rulerNames[] = {"The Conqueror", "The Agrarian Sage", "The Merchant Prince", "The Strategist"};
	const std::string kingdomNames[] = {"Anuradhapura", "Ruhuna", "Kotte", "Kandy"};
	const RulerEnum enums[] = {ANURADHAPURA, RUHUNA, KOTTE, KANDY};
	const std::vector<std::pair<int, int>> kingdomCapitals = {std::make_pair(0, 0), std::make_pair(0, 5), std::make_pair(5, 0), std::make_pair(5, 5)};
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		Kingdom kingdom(kingdomNames[i]);
		Territory* capital = worldMap.getTerritory(kingdomCapitals[i].first, kingdomCapitals[i].second);
		std::unique_ptr<Building> building = Builder::createBuilding(BARRACKS);
		capital->buildAProperty(building);
		kingdom.increaseSpecificBuildingCount(BARRACKS);
		kingdom.claimTerritory(capital);

		switch (i)
		{
			case 0:
				rulers.push_back(std::make_unique<Conqueror>(rulerNames[i], kingdom, enums[i]));
				break;
			case 1:
				rulers.push_back(std::make_unique<AgrarianSage>(rulerNames[i], kingdom, enums[i]));
				break;
			case 2:
				rulers.push_back(std::make_unique<MerchantPrince>(rulerNames[i], kingdom, enums[i]));
				break;
			case 3:
				rulers.push_back(std::make_unique<Strategist>(rulerNames[i], kingdom, enums[i]));
				break;
		}
	}

	initializeGame();
}

void SimulationManager::initializeGame()
{
	printTitle(" KINGDOM OF LANKA Simulation ", '=');
    std::cout << "Seed : " << SEED << std::endl << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 1 : Anuradhapura" << " - The Conqueror" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 2 : Ruhuna" << " - The Agrarian Sage" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 3 : Kotte" << " - The Merchant Prince" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 4 : Kandy" << " - The Strategist" << std::endl;

	std::cout << std::endl << "Each kingdom begins with 1000 Gold, 500 Rice, 200 Timber, 100 Iron." << std::endl;
	
	determinePlayerOrder(rulers);
	std::sort(rulers.begin(), rulers.end(), [](const std::unique_ptr<Ruler>& a, const std::unique_ptr<Ruler>& b) {
		return a->getFirstDieValue() > b->getFirstDieValue();
	});
	std::cout << "Turn order : ";
	int count = 0;
	for (auto& R : rulers)
	{
		std::cout << R->getKingdom().getKingdomName();
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
		for (auto& R : rulers)
		{
			Kingdom& kingdom = R->getKingdom();
			std::string title = " Round " + std::to_string(gameRound + 1) + " : " + kingdom.getKingdomName() + " ";
			std::cout << std::endl;
			printTitle(title, '-');
			// collect productions
			kingdom.collectProductions(worldMap, R->getRulerEnum());
			// upkeep and consumption
			kingdom.upkeepAndConsumption();
			// event check
			// to be implemented
			// ruler turns
			std::vector<RulerDetails> rd = calculateRulerDetails();
			R->playTurn(worldMap, rd);
		}

		// printing round summary
		std::string title = " Round " + std::to_string(gameRound + 1) + " Summary ";
		std::cout << std::endl;
		printTitle(title, '=');
		for (auto& R : rulers)
		{
			R->printRoundSummary();
			std::cout << std::string(MESSAGE_WIDTH, '-') << std::endl;
		}
		gameRound++;
	}

	endGame();
}

void SimulationManager::endGame()
{
	// printing game over message and wounding up
	std::sort(rulers.begin(), rulers.end(), [](std::unique_ptr<Ruler>& a, std::unique_ptr<Ruler>& b){
		return a->calculateScore() > b->calculateScore();
	});
	std::cout << std::endl;
	std::string title = " Game Over after " + std::to_string(gameRound) + " rounds ";
	printTitle(title, '=');

	std::cout << "Winner : " << rulers[0]->getKingdom().getKingdomName() << " (" << rulers[0]->getRulerName() << ")" << std::endl << std::endl;
	std::cout << "Final Score" << std::endl;
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		std::cout << i + 1 << ". " << std::setw(12) << rulers[i]->getKingdom().getKingdomName() << " : " << rulers[i]->calculateScore() << std::endl;
	}
}
		
std::vector<RulerDetails> SimulationManager::calculateRulerDetails()
{
	std::vector<RulerDetails> rulerDetails;

	for (auto& R : rulers)
	{
		RulerDetails rd;
		rd.noOfTerritoriesOwn = R->getKingdom().getNoOfTerritoriesOwned();
		rd.armyStrength = 0;
		rulerDetails.push_back(rd);
	}

	return rulerDetails;
}
