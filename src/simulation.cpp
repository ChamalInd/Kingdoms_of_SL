#include "../includes/includes.h"
		
SimulationManager::SimulationManager()
{
	// creating kingdoms
	const std::string kingdomNames[] = {"Anuradhapura", "Ruhuna", "Kotte", "Kandy"};
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		Kingdom kingdom(kingdomNames[i]);
		kingdoms.push_back(kingdom);
	}

	// creating world map
	int territoryID = 0;
	int reservedCount = NO_OF_KINGDOMS;
	std::vector<int> terrainCount = setTheTerrainCounts(); 
	TerrainType terrains[] = {PADDY_PLAIN, HILL_COUNTRY, DRY_ZONE, COAST, JUNGLE, LAKE};
	
	for (int i = 0; i < MAP_DIMENSIONS; i++)
	{
		for (int j = 0; j < MAP_DIMENSIONS; j++)
		{
			std::string territoryName = "Territory " + std::to_string(territoryID);
			Ruler ruler = NEUTRAL;
			TerrainType terrain;

			// setting up the capital territories
			if (i == 0)
			{
				if (j == 0) ruler = ANURADHAPURA;
				else if (j == 5) ruler = RUHUNA;
			}
			else if (i == 5)
			{
				if (j == 0) ruler = KOTTE;
				else if (j == 5) ruler = KANDY;
			}
			
			//  deciding the terrain type
			if (ruler != NEUTRAL) 
			{
				terrain = PADDY_PLAIN;
				reservedCount--;
			}
			else
			{
				while (true)
				{
					terrain = terrains[rand() % NO_OF_TERRAINS];
					if (terrain == PADDY_PLAIN && terrainCount[terrain] <= reservedCount) continue;
					if (terrainCount[terrain] >= 1) break;
				}
			}

			terrainCount[terrain]--;
					
			// creating the territory object
			Territory territory(territoryID, territoryName, terrain, ruler);
			worldMap[i].push_back(territory);
			
			territoryID++;
		}
	}

	initializeGame();
}

void SimulationManager::initializeGame()
{
	std::cout << std::string(20, '=') << " KINGDOM OF LANKA Simulation " << std::string(20, '=') << std::endl << std::endl;
    std::cout << "Seed : " << SEED << std::endl << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 1 : Anuradhapura" << " - The Conqueror" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 2 : Ruhuna" << " - The Agrarian Sage" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 3 : Kotte" << " - The Merchant Prince" << std::endl;
	std::cout << std::left << std::setw(24) << "Kingdom 4 : Kandy" << " - The Strategist" << std::endl;

	std::cout << std::endl << "Each kingdom begins with 1000 Gold, 500 Rice, 200 Timber, 100 Iron." << std::endl;
	
	determinePlayerOrder(kingdoms);
	std::sort(kingdoms.begin(), kingdoms.end(), [](const Kingdom& a, const Kingdom& b) {
		return a.getFirstDieValue() > b.getFirstDieValue();
	});
	std::cout << "Turn order : ";
	int count = 0;
	for (Kingdom K : kingdoms)
	{
		std::cout << K.getKingdomName();
		count++;
		if (count < NO_OF_KINGDOMS) std::cout << ", ";
	}
	std::cout << "." << std::endl;

}
