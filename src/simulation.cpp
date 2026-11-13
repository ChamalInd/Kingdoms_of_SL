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
		Kingdom kingdom(kingdomNames[i]);

		// preparing capital with a barracks and an army
		Territory* capital = worldMap.getTerritory(kingdomCapitals[i].first, kingdomCapitals[i].second);
		std::unique_ptr<Building> building = BuildingMaker::createBuilding(BARRACKS);
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
	while (true)
	{
		std::cout << std::endl;

		for (int i = 0; i < NO_OF_KINGDOMS; i++)
		{	
			if (rulers[i]->getPlaceChosen()) continue;
			int dieRoll = rollDie();
			rulers[i]->setFirstDieValue(dieRoll);
			std::cout << rulers[i]->getKingdom().getKingdomName() << " rolls " << dieRoll << "." << std::endl;
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
				std::cout << rulers[i]->getKingdom().getKingdomName() << " ";
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
		logger.printTitle(" Round " + std::to_string(gameRound + 1), ' ');
		std::cout << std::string(MESSAGE_WIDTH, '=') << std::endl;

		eventHandler();		

		for (auto& R : rulers)
		{
			Kingdom& kingdom = R->getKingdom();
			std::string title = " Round " + std::to_string(gameRound + 1) + " : " + kingdom.getKingdomName() + " ";
			std::cout << std::endl;
			logger.printTitle(title, '-');
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

		currentEvent = EventCreater::createEvent(eventList[rand() % 7]);
		std::cout << std::endl;
		logger.printTitle(" New Event Occurred ", '*');
		std::cout << "Event Name: " << currentEvent->getEventName() << std::endl;
		std::cout << "Event Description: " << currentEvent->getEventEffect() << std::endl;

		// immediate events
		if (currentEvent->getEventType() == FLOOD)
		{
			Territory* selected = nullptr;
			int tries = 0;
			while (true)
			{
				int x = rand() % 6;
				int y = rand() % 6;
				if (x == 0 && y == 0) continue;
				if (x == 0 && y == 5) continue;
				if (x == 5 && y == 0) continue;
				if (x == 5 && y == 5) continue;
				selected = worldMap.getTerritory(x, y);
				if (selected->getTerritoryBuilding() == nullptr) continue;
				if (selected->getTerritoryBuilding()->getBuildingType() == FORT) continue;
				if (selected->getTerritoryType() != PADDY_PLAIN && selected->getTerritoryType() != COAST) continue;
				if (++tries > NO_OF_TILES) break;
				break;
			}
					
			if (tries <= NO_OF_TILES)
			{
				for (int i = 0; i < NO_OF_KINGDOMS; i++)
				{
					if (selected->getTerritoryRuler() == rulers[i]->getRulerEnum())
					{
						rulers[i]->getKingdom().decreaseSpecificBuildingCount(selected->getTerritoryBuilding()->getBuildingType());
						break;
					}
				}
				std::cout << selected->getTerritoryBuilding()->getBuildingName() << " on " << selected->getTerritoryName() << " destroyed by flood." << std::endl;
				selected->destroyAProperty();
			}
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
