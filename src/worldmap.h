#pragma once
#include "includes.h"
#include "territory.h"

class WorldMap
{
	private:
		std::vector<std::vector<Territory>> worldMap{MAP_DIMENSIONS};
	
	public:
		WorldMap();
		
		Territory* getTerritory(int i, int j) { return &worldMap[i][j]; };
		
		std::vector<int> setTheTerrainCounts();
		void printWorldMap();
};
