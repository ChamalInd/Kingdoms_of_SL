#include "../includes/building.h"

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
		
		resources[0] += T->getTerritoryProduction()[0];
		if (T->getTerritoryBuilding() != nullptr && T->getTerritoryBuilding()->getBuildingType() == PADDY_FARM) resources[0] += 30;
		resources[0] = resources[0] * 50 / 100;
	}

	return Resources(resources);
}
