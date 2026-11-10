#include "../includes/kingdom.h"

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
		for (int i = 0; i < 4; i++) 
		{
			int reduction = 0;
			if (stability < 30)
			{
				reduction = baseProduction[i] * (25 / 100);
			}
			resourceIncrements[i] += (baseProduction[i] - reduction);
		}
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

	if (consumption <= currentResources[0])
	{
		resourceIncrement[0] = 0 - (population / 20);
	}

	if ((consumption * 2) <= currentResources[0])
	{
		population += population * (2 / 100);
		updateStability(1);
	}
	
	if (consumption >= currentResources[0])
	{
		resourceIncrement[0] = 0 - currentResources[0];
		population -= population * (5 / 100);
		updateStability(-10);
	}

	// taxes
	resourceIncrement[3] = population / 25;

	resources += resourceIncrement;

	std::cout << kingdomName << " population consumed: Rice " << resourceIncrement[0] << "." << std::endl;
	std::cout << kingdomName << " population paid taxes: Gold +" << resourceIncrement[3] << "." << std::endl;
	
}

void Kingdom::updateStability(int rate)
{
	stability = std::clamp(stability + rate, 0, 100);

	if (stability <= 0)
	{
		if (getNoOfTerritoriesOwned() > 1)
		{
			int randomTerritory = rand() % (getNoOfTerritoriesOwned() - 1) + 1;
			ownedTerritories[randomTerritory]->setTerritoryRuler(NEUTRAL);
			ownedTerritories.erase(ownedTerritories.begin() + randomTerritory);
		}
		stability = 30;
	}
}
