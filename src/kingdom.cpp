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
		
void Kingdom::upkeepAndConsumption()
{
	std::vector<int> resourceIncrement = {0, 0, 0, 0};
	std::vector<int> currentResources = resources.getResources();

	// consume rice
	int consumption = population / 20;

	if (consumption < currentResources[1])
	{
		resourceIncrement[0] = 0 - (population / 20);
	}

	if ((consumption * 2) < currentResources[1])
	{
		population += population * 0.2;
	}
	
	if (consumption > currentResources[1])
	{
		resourceIncrement[0] = 0 - currentResources[1];
		population -= population * 0.5;
		stability -= 10;
	}

	// taxes
	resourceIncrement[3] = population / 25;

	resources += resourceIncrement;

	std::cout << kingdomName << " population consumed: Rice " << resourceIncrement[0] << "." << std::endl;
	std::cout << kingdomName << " population paid taxes: Gold +" << resourceIncrement[1] << "." << std::endl;
	
}
