#include "../includes/includes.h"

Kingdom::Kingdom(std::string n)
{
	kingdomName = n;
}

void Kingdom::collectProductions()
{
	std::vector<int> resourceIncrements = {0, 0, 0, 0};
	for (Territory* T : ownedTerritories)
	{
		std::vector<int> baseProduction = T->getTerritoryProduction();
		for (int i = 0; i < 4; i++) resourceIncrements[i] += baseProduction[i];
	}

	resources += resourceIncrements;

	std::cout << kingdomName << " produced: Gold +" << resourceIncrements[3] << ", Rice +" << resourceIncrements[0] << ", Timber +" << resourceIncrements[1] << ", Iron +" << resourceIncrements[2] << "." << std::endl;
}
