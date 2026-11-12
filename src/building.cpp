#include "../includes/building.h"
#include "../includes/territory.h"
#include "../includes/worldmap.h"

BuildingEffect Building::performEffect(Territory* parentTerritory, WorldMap& worldMap, RulerEnum ruler)
{
	BuildingEffect effect;
	
	effect.resourceIncrement = getResourceIncrements(parentTerritory, worldMap, ruler);
	effect.stabilityIncrement = getStabilityIncrements();

	return effect;
}

Resources IrrigationTank::getResourceIncrements(Territory* parentTerritory, WorldMap& map, RulerEnum ruler) const
{
	std::vector<std::pair<int, int>> neighbours = parentTerritory->getTerritoryNeighbours();
	std::vector<int> resources = {0, 0, 0, 0};
	for (auto P : neighbours)
	{
		Territory* T = map.getTerritory(P.first, P.second);
		if (T == nullptr) continue;
		if (T->getTerritoryRuler() != ruler) continue;
		if (T->getTerritoryType() != PADDY_PLAIN && T->getTerritoryType() != DRY_ZONE) continue;
		
		resources[RICE] += T->getTerritoryProduction()[RICE];
		if (T->getTerritoryBuilding() != nullptr && T->getTerritoryBuilding()->getBuildingType() == PADDY_FARM) resources[RICE] += 30;
		resources[RICE] = resources[RICE] * 50 / 100;
	}

	if (buildingHealth <= 50) resources[RICE] /= 2;

	return Resources(resources);
}
