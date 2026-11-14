#include "territory.h"
	
Territory::Territory(std::string n, TerrainType t, RulerEnum r, std::pair<int, int> location) : territoryName(n), territoryType(t), territoryRuler(r)
{
	territoryBaseProduction = BASE_PRODUCTION[t];

	// filtering neighbours
	int adders[] = {-1, 0, 1};
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 0 && j == 0) continue;
			if (i == 2 && j == 2) continue;
			if (i == 0 && j == 2) continue;
			if (i == 2 && j == 0) continue;
			if (location.first + adders[i] < 0 || location.first + adders[i] > 5) continue;
			if (location.second + adders[j] < 0 || location.second + adders[j] > 5) continue;
			if (adders[i] == 0 && adders[j] == 0) continue;
			territoryNeighbours.push_back(std::make_pair(location.first + adders[i], location.second + adders[j]));
		}
	}
}
