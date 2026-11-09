#include "../includes/includes.h"

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

void determinePlayerOrder(std::vector<Kingdom>& kingdoms)
{
	while (true)
	{
		std::cout << std::endl;

		for (Kingdom& K : kingdoms)
		{	
			if (K.getPlaceChosen()) continue;
			int dieRoll = rollDie(VALUE);
			K.setFirstDieValue(dieRoll);
			std::cout << K.getKingdomName() << " rolls " << dieRoll << "." << std::endl;
		}
	
		bool tieFound = false;
		for (int i = 0; i < NO_OF_KINGDOMS; i++)
		{
			if (kingdoms[i].getPlaceChosen()) continue;
			tieFound = false;
			for (int j = 0; j < NO_OF_KINGDOMS; j++)
			{
				if (i == j) continue;
				if (kingdoms[j].getPlaceChosen()) continue;
				if (kingdoms[i].getFirstDieValue() == kingdoms[j].getFirstDieValue()) 
				{ 
					tieFound = true;
					break;
				}
			}

			if (!tieFound) kingdoms[i].setPlaceChosen(true);
		}


		int tieCount = 0;
		std::cout << "Tie between ";
		for (Kingdom& K : kingdoms)
		{
			if (!K.getPlaceChosen()) 
			{
				tieCount++;
				std::cout << K.getKingdomName() << " ";
			}
			else K.setFirstDieValue(K.getFirstDieValue() * 10);
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
