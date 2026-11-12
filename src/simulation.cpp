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

		// preparing capital with a barracks and an army
		Territory* capital = worldMap.getTerritory(kingdomCapitals[i].first, kingdomCapitals[i].second);
		std::unique_ptr<Building> building = Builder::createBuilding(BARRACKS);
		capital->buildAProperty(building);
		kingdom.increaseSpecificBuildingCount(BARRACKS);
		
		kingdom.increaseArmyCount();
		capital->addAnArmy(std::make_unique<Army>(kingdom.getArmyCount()));
	
		kingdom.claimTerritory(capital);
		rulers.push_back(RulerMaker::createRulers(rulerNames[i], kingdom, enums[i]));
	}

	initializeGame();
}

void SimulationManager::initializeGame()
{
	std::cout << std::string(70, '=') << std::endl;
	printTitle(" KINGDOM OF LANKA Simulation ", ' ');
//    std::cout << "Seed : " << SEED << std::endl;
	std::cout << std::string(70, '=') << std::endl << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 1 : Anuradhapura" << std::setw(22) << " - The Conqueror" << " Capital at (0, 0)" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 2 : Ruhuna" << std::setw(22) << " - The Agrarian Sage" << " Capital at (0, 5)" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 3 : Kotte" << std::setw(22) << " - The Merchant Prince" << " Capital at (5, 0)" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 4 : Kandy" << std::setw(22) << " - The Strategist" << " Capital at (5, 0)" << std::endl;

	std::cout << std::endl << "Each kingdom begins with 1000 Gold, 500 Rice, 200 Timber, 100 Iron." << std::endl;
	std::cout << "Population: 1000 | Stability: 70" << std::endl;

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
}

void SimulationManager::gameLoop()
{

	while (gameRound < 200)
	{
		std::cout << std::endl << std::string(MESSAGE_WIDTH, '=') << std::endl;
		printTitle(" Round " + std::to_string(gameRound + 1), ' ');
		std::cout << std::string(MESSAGE_WIDTH, '=') << std::endl;

		eventHandler();		

		for (auto& R : rulers)
		{
			Kingdom& kingdom = R->getKingdom();
			std::string title = " Round " + std::to_string(gameRound + 1) + " : " + kingdom.getKingdomName() + " ";
			std::cout << std::endl;
			printTitle(title, '-');
			// collect productions
			kingdom.collectProductions(worldMap, R->getRulerEnum(), currentEvent);
			// upkeep and consumption
			kingdom.populationActivities(currentEvent);
			kingdom.buildingUpkeep();
			kingdom.armyUpkeep();
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

		if (currentEvent != nullptr) 
		{
			int roundsLeft = currentEvent->getEventRounds();
			if (roundsLeft > 0) 
			{
				std::cout <<  "Active Events: " << currentEvent->getEventName() << " (" << roundsLeft << " rounds remaining)" << std::endl;
				std::cout << std::string(MESSAGE_WIDTH, '-') << std::endl;
			}
		}

		gameRound++;
	}

	endGame();
}

void SimulationManager::eventHandler()
{
	// event check
	if (currentEvent == nullptr && (gameRound + 1) % 10 == 0)
	{
		std::vector<EventType> eventList = {DROUGHT, FLOOD, GOOD_MONSOON, PLAGUE, TRADE_CARAVAN, ROYAL_FESTIVAL, F_O_IRON_AGE};

		currentEvent = EventCreater::createEvent(eventList[rand() % 7]);
		std::cout << std::endl;
		printTitle(" New Event Occurred ", '*');
		std::cout << "Event Name: " << currentEvent->getEventName() << std::endl;
		std::cout << "Event Description: " << currentEvent->getEventEffect() << std::endl;

		// immediate events
		if (currentEvent->getEventType() == FLOOD)
		{
			Territory* selected = nullptr;
			while (true)
			{
				int x = rand() % 6;
				int y = rand() % 6;
				selected = worldMap.getTerritory(x, y);
				if (selected->getTerritoryBuilding() == nullptr) continue;
				if (selected->getTerritoryBuilding()->getBuildingType() == FORT) continue;
				if (selected->getTerritoryType() != PADDY_PLAIN && selected->getTerritoryType() != COAST) continue;
				break;
			}
					
			std::cout << selected->getTerritoryBuilding()->getBuildingName() << " on " << selected->getTerritoryName() << " destroyed by flood." << std::endl;
			selected->destroyAProperty();
		}
		
		if (currentEvent->getEventType() == TRADE_CARAVAN)
		{
			int minimum = INT_MAX;
			std::vector<int> indices;
			for (int i = 0; i < NO_OF_KINGDOMS; i++)
			{
				if (rulers[i]->getKingdom().getNoOfTerritoriesOwned() <= minimum)
				{
					if (rulers[i]->getKingdom().getNoOfTerritoriesOwned() < minimum) indices.clear();
					indices.push_back(i);
					minimum = rulers[i]->getKingdom().getNoOfTerritoriesOwned();
				}
			}
			for (int index : indices)
			{
				std::cout << rulers[index]->getKingdom().getKingdomName() << " received 500 Gold from the Trade Caravan." << std::endl;
				rulers[index]->getKingdom().getResourceObject().addToStock(GOLD, 500);
			}
		}
	}
	else if (currentEvent != nullptr) 
	{
		currentEvent->decreaseEventRound();
		int roundsLeft = currentEvent->getEventRounds();
		if (roundsLeft == 0) 
		{
			std::cout << std::endl;
			printTitle(" Event Ended ", '*');
			std::cout << "Event Name: " << currentEvent->getEventName() << std::endl;
			std::cout << "Event Description: " << currentEvent->getEventOverMsg() << std::endl;
			currentEvent = nullptr;
		}
	}
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
		
		for (auto& T : R->getKingdom().getOwnedTerritories())
		{
			if (T->getTerritoryArmy() == nullptr) continue;
			rd.armyStrength += T->getTerritoryArmy()->getArmyStrength(0);
		}

		rulerDetails.push_back(rd);
	}

	return rulerDetails;
}
