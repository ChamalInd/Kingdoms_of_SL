#include "logger.h"

void Logger::printTitle(std::string title, char decorator)
{
	int lPad = (MESSAGE_WIDTH - title.length()) / 2;
	int rPad = MESSAGE_WIDTH - title.length() - lPad;
	std::cout << std::string(lPad, decorator) << title << std::string(rPad, decorator) << std::endl;
}

void Logger::printSimulationInitializer()
{
	std::cout << std::string(70, '=') << std::endl;
	printTitle(" KINGDOM OF LANKA Simulation ", ' ');
//    std::cout << "Seed : " << SEED << std::endl;
	std::cout << std::string(70, '=') << std::endl << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 1 : Anuradhapura" << std::setw(22) << " - The Conqueror" << " Capital at (0, 0)" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 2 : Ruhuna" << std::setw(22) << " - The Agrarian Sage" << " Capital at (0, 5)" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 3 : Kotte" << std::setw(22) << " - The Merchant Prince" << " Capital at (5, 0)" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 4 : Kandy" << std::setw(22) << " - The Strategist" << " Capital at (5, 5)" << std::endl;

	std::cout << std::endl << "Each kingdom begins with 1000 Gold, 500 Rice, 200 Timber, 100 Iron." << std::endl;
	std::cout << "Population: 1000 | Stability: 70" << std::endl;

}
		
void Logger::printSimulationEnd(const int& gameRound, std::vector<std::unique_ptr<Ruler>>& rulers)
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

void Logger::printRoundSummary(std::vector<std::unique_ptr<Ruler>>& rulers, std::unique_ptr<Event>& currentEvent, const int& gameRound)
{
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
}
