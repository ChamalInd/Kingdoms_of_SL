#include "building.h"
#include "territory.h"
#include "worldmap.h"

BuildingEffect PaddyFarm::performEffect(Territory*, WorldMap&, RulerEnum)
{
	BuildingEffect effect;

	if (buildingHealth <= 50) effect.resourceIncrement = Resources({15, 0, 0, 0});
	else effect.resourceIncrement = Resources({30, 0, 0, 0}); 
	
	effect.stabilityIncrement = 0;

	return effect;
}

BuildingEffect Mine::performEffect(Territory*, WorldMap&, RulerEnum)
{
	BuildingEffect effect;

	if (buildingHealth <= 50) effect.resourceIncrement = Resources({0, 0, 10, 0});
	else effect.resourceIncrement = Resources({0, 0, 20, 0}); 
	
	effect.stabilityIncrement = 0;

	return effect;
}

BuildingEffect SawMill::performEffect(Territory*, WorldMap&, RulerEnum)
{
	BuildingEffect effect;

	if (buildingHealth <= 50) effect.resourceIncrement = Resources({0, 15, 0, 0});
	else effect.resourceIncrement = Resources({0, 30, 0, 0}); 
	
	effect.stabilityIncrement = 0;

	return effect;
}

BuildingEffect Market::performEffect(Territory*, WorldMap&, RulerEnum)
{
	BuildingEffect effect;

	if (buildingHealth <= 50) effect.resourceIncrement = Resources({0, 0, 0, 20});
	else effect.resourceIncrement = Resources({0, 0, 0, 40}); 
	
	effect.stabilityIncrement = 0;

	return effect;
}

BuildingEffect Temple::performEffect(Territory*, WorldMap&, RulerEnum)
{
	BuildingEffect effect;
	
	effect.resourceIncrement = Resources({0, 0, 0, 0});
	effect.stabilityIncrement = 3;

	return effect;
}

BuildingEffect IrrigationTank::performEffect(Territory* parentTerritory, WorldMap& map, RulerEnum ruler)
{
	BuildingEffect effect;
	
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

	effect.resourceIncrement = Resources(resources);
	effect.stabilityIncrement = 0;

	return effect;
}
