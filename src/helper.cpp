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
