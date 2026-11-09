#include "../includes/includes.h"

WorldMap::WorldMap()
{
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
			std::pair<int, int> location = std::make_pair(i, j);

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
			Territory territory(territoryID, territoryName, terrain, ruler, location);
			worldMap[i].push_back(territory);
			
			territoryID++;
		}
	}
}
