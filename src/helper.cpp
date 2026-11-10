#include "../includes/ruler.h"

std::vector<int> setTheTerrainCounts()
{
	std::vector<int> counts = {0, 0, 0, 0, 0, 0};
	int total = 0;

	while (total < NO_OF_TILES)
	{
		counts[rand() % NO_OF_TERRAINS]++;
		total++;
	}

	while (counts[0] < 4)
	{
		counts[0]++;
		counts[rand() % (NO_OF_TERRAINS - 1) + 1]--;
	}

	return counts;
}

int rollDie(RollType type)
{
	int die_1 = rand() % 6 + 1;
	int die_2 = rand() % 6 + 1;

	if (type == VALUE) return die_1 + die_2;
	else return die_1 == die_2;
}

void printTitle(std::string title, char decorator)
{
	int lPad = (MESSAGE_WIDTH - title.length()) / 2;
	int rPad = MESSAGE_WIDTH - title.length() - lPad;
	std::cout << std::string(lPad, decorator) << title << std::string(rPad, decorator) << std::endl;
}

void determinePlayerOrder(std::vector<std::unique_ptr<Ruler>>& rulers)
{
	while (true)
	{
		std::cout << std::endl;

		for (int i = 0; i < NO_OF_KINGDOMS; i++)
		{	
			if (rulers[i]->getPlaceChosen()) continue;
			int dieRoll = rollDie(VALUE);
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
}
