#include "building.h"
#include "territory.h"
#include "worldmap.h"

Resources PaddyFarm::getResourceEffect(Territory*, WorldMap&, RulerEnum) const
{
	if (buildingHealth <= 50) return Resources({15, 0, 0, 0});
	return Resources({30, 0, 0, 0}); 
}

Resources Mine::getResourceEffect(Territory*, WorldMap&, RulerEnum) const
{
	if (buildingHealth <= 50) return Resources({0, 0, 10, 0});
	return Resources({0, 0, 20, 0}); 
}

Resources SawMill::getResourceEffect(Territory*, WorldMap&, RulerEnum) const
{
	if (buildingHealth <= 50) return Resources({0, 15, 0, 0});
	return Resources({0, 30, 0, 0}); 
}

Resources Market::getResourceEffect(Territory*, WorldMap&, RulerEnum) const
{
	if (buildingHealth <= 50) return Resources({0, 0, 0, 20});
	return Resources({0, 0, 0, 40}); 
}

Resources IrrigationTank::getResourceEffect(Territory* parentTerritory, WorldMap& map, RulerEnum ruler) const
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
	}

	resources[RICE] = resources[RICE] * 50 / 100;

	if (buildingHealth <= 50) resources[RICE] /= 2;

	return Resources(resources);
}
