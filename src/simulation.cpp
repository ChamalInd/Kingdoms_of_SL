#include "simulation.h"
		
SimulationManager::SimulationManager()
{
	// creating kingdoms and their rulers
	const std::string rulerNames[] = {"The Conqueror", "The Agrarian Sage", "The Merchant Prince", "The Strategist"};
	const std::string kingdomNames[] = {"Anuradhapura", "Ruhuna", "Kotte", "Kandy"};
	const RulerEnum enums[] = {ANURADHAPURA, RUHUNA, KOTTE, KANDY};
	const std::vector<std::pair<int, int>> kingdomCapitals = {std::make_pair(0, 0), std::make_pair(0, 5), std::make_pair(5, 0), std::make_pair(5, 5)};
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		std::unique_ptr<Kingdom> kingdom = std::make_unique<Kingdom>(kingdomNames[i], enums[i]);

		// preparing capital with a barracks and an army
		Territory* capital = worldMap.getTerritory(kingdomCapitals[i].first, kingdomCapitals[i].second);
		std::unique_ptr<Building> building = BuildingFactory::createBuilding(BARRACKS);
		kingdom->claimTerritory(capital, 0);
		kingdom->constructBuilding(capital, building);
	
		rulers.push_back(RulerFactory::createRulers(rulerNames[i], std::move(kingdom), enums[i]));
	}

	initializeGame();
}

void SimulationManager::initializeGame()
{
	logger.printSimulationInitializer();
	while (true)
	{
		std::cout << std::endl;

		for (int i = 0; i < NO_OF_KINGDOMS; i++)
		{	
			if (rulers[i]->getPlaceChosen()) continue;
			int dieRoll = rollDie();
			rulers[i]->setFirstDieValue(dieRoll);
			std::cout << rulers[i]->getKingdom()->getKingdomName() << " rolls " << dieRoll << "." << std::endl;
		}
	
		bool tieFound = false;
		for (int i = 0; i < NO_OF_KINGDOMS; i++)
		{
			if (rulers[i]->getPlaceChosen()) continue;
			tieFound = false;
			for (int j = 0; j < NO_OF_KINGDOMS; j++)
			{
				if (i == j) continue;
				if (rulers[j]->getPlaceChosen()) continue;
				if (rulers[i]->getFirstDieValue() == rulers[j]->getFirstDieValue()) 
				{ 
					tieFound = true;
					break;
				}
			}

			if (!tieFound) rulers[i]->setPlaceChosen(true);
		}


		int tieCount = 0;
		std::cout << "Tie between ";
		for (int i = 0; i < NO_OF_KINGDOMS; i++)
		{
			if (!rulers[i]->getPlaceChosen()) 
			{
				tieCount++;
				std::cout << rulers[i]->getKingdom()->getKingdomName() << " ";
			}
			else rulers[i]->setFirstDieValue(rulers[i]->getFirstDieValue() * 10);
		}

		if (tieCount == 0)
		{
			std::cout << std::string(12, '\b') << std::string(12, ' ');
			break;
		}
		else
		{
			std::cout << "\b. Re-rolling..." << std::endl;
		}
	}
	std::cout << std::endl;

	std::sort(rulers.begin(), rulers.end(), [](const std::unique_ptr<Ruler>& a, const std::unique_ptr<Ruler>& b) {
		return a->getFirstDieValue() > b->getFirstDieValue();
	});
	std::cout << "Turn order : ";
	int count = 0;
	for (auto& R : rulers)
	{
		std::cout << R->getKingdom()->getKingdomName();
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
		logger.printTitle(" Round " + std::to_string(gameRound + 1), ' ');
		std::cout << std::string(MESSAGE_WIDTH, '=') << std::endl;

		eventHandler();		

		for (auto& R : rulers)
		{
			std::unique_ptr<Kingdom>& kingdom = R->getKingdom();
			std::string title = " Round " + std::to_string(gameRound + 1) + " : " + kingdom->getKingdomName() + " ";
			std::cout << std::endl;
			logger.printTitle(title, '-');
			// collect productions
			kingdom->collectProductions(worldMap, R->getRulerEnum(), currentEvent);
			// upkeep and consumption
			kingdom->populationActivities(currentEvent);
			kingdom->updateStability(currentEvent);
			kingdom->buildingUpkeep();
			kingdom->armyUpkeep();
			// event check
			// to be implemented
			// ruler turns
			std::vector<RulerDetails> rd = calculateRulerDetails();
			R->playTurn(worldMap, rd, currentEvent);
		}

		logger.printRoundSummary(rulers, currentEvent, gameRound);	
		gameRound++;
	}

	logger.printSimulationEnd(gameRound, rulers);
}

void SimulationManager::eventHandler()
{
	// event check
	if (currentEvent == nullptr && (gameRound + 1) % 10 == 0)
	{
		std::vector<EventType> eventList = {DROUGHT, FLOOD, GOOD_MONSOON, PLAGUE, TRADE_CARAVAN, ROYAL_FESTIVAL, F_O_IRON_AGE};

		currentEvent = EventFactory::createEvent(eventList[rand() % 7]);
		std::cout << std::endl;
		logger.printTitle(" New Event Occurred ", '*');
		std::cout << "Event Name: " << currentEvent->getEventName() << std::endl;
		std::cout << "Event Description: " << currentEvent->getEventEffect() << std::endl;

		currentEvent->applyTradeCaravanEffect(rulers);
		currentEvent->applyFloodEffect(worldMap, rulers);
	}
	else if (currentEvent != nullptr) 
	{
		currentEvent->decreaseEventRound();
		int roundsLeft = currentEvent->getEventRounds();
		if (roundsLeft == 0) 
		{
			std::cout << std::endl;
			logger.printTitle(" Event Ended ", '*');
			std::cout << "Event Name: " << currentEvent->getEventName() << std::endl;
			std::cout << "Event Description: " << currentEvent->getEventOverMsg() << std::endl;
			currentEvent = nullptr;
		}
	}
}

int SimulationManager::rollDie()
{
	int die_1 = rand() % 6 + 1;
	int die_2 = rand() % 6 + 1;

	return die_1 + die_2;
}

std::vector<RulerDetails> SimulationManager::calculateRulerDetails()
{
	std::vector<RulerDetails> rulerDetails;

	for (auto& R : rulers)
	{
		RulerDetails rd;
		rd.ruler = R->getRulerEnum();
		rd.noOfTerritoriesOwn = R->getKingdom()->getNoOfTerritoriesOwned();
		rd.armyStrength = 0;
		
		for (auto& A : R->getKingdom()->getOwnedArmies())
		{
			rd.armyStrength += A->getArmyStrength(0);
		}

		rulerDetails.push_back(rd);
	}

	return rulerDetails;
}
