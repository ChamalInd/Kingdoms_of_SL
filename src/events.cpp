#include "events.h"


ResourceEffect Event::applyResourceEffect() const 
{
	ResourceEffect effect; 
	effect.resource = RICE;
	effect.scale = 100;
	return effect;
}

ResourceEffect Drought::applyResourceEffect() const 
{
 	ResourceEffect effect; 
	effect.resource = RICE;
	effect.scale = 50;
	return effect;
}

ResourceEffect GoodMonsoon::applyResourceEffect() const 
{
 	ResourceEffect effect; 
	effect.resource = RICE;
	effect.scale = 150;
	return effect;
}

ResourceEffect FamineIron::applyResourceEffect() const 
{
 	ResourceEffect effect; 
	effect.resource = IRON;
	effect.scale = 50;
	return effect;
}

PopulationEffect Event::applyPopulationEffect() const
{
	PopulationEffect effect;
	effect.populationScale = 100;
	effect.consumptionScale = 100;
	return effect;
}

PopulationEffect Plague::applyPopulationEffect() const
{
	PopulationEffect effect;
	effect.populationScale = 90;
	effect.consumptionScale = 100;
	return effect;
}

PopulationEffect RoyalFestival::applyPopulationEffect() const
{
	PopulationEffect effect;
	effect.populationScale = 100;
	effect.consumptionScale = 150;
	return effect;
}
		
void TradeCaravan::applyTradeCaravanEffect(std::vector<std::unique_ptr<Ruler>>& rulers) const
{
	int minimum = INT_MAX;
	std::vector<int> indices;
	for (int i = 0; i < NO_OF_KINGDOMS; i++)
	{
		if (rulers[i]->getKingdom()->getNoOfTerritoriesOwned() <= minimum)
		{
			if (rulers[i]->getKingdom()->getNoOfTerritoriesOwned() < minimum) indices.clear();
			indices.push_back(i);
			minimum = rulers[i]->getKingdom()->getNoOfTerritoriesOwned();
		}
	}
	for (int index : indices)
	{
		std::cout << rulers[index]->getKingdom()->getKingdomName() << " received 500 Gold from the Trade Caravan." << std::endl;
		rulers[index]->getKingdom()->getResourceObject().addToStock(GOLD, 500);
	}
}
		
void Flood::applyFloodEffect(WorldMap& worldMap, std::vector<std::unique_ptr<Ruler>>& rulers) const
{	
	Territory* selected = nullptr;
	std::vector<std::pair<int, int>> triedPairs;
	while (triedPairs.size() < MAP_DIMENSIONS * MAP_DIMENSIONS - 4)
	{
		int x = rand() % MAP_DIMENSIONS;
		int y = rand() % MAP_DIMENSIONS;
		if (x == 0 && y == 0) continue;
		if (x == 0 && y == 5) continue;
		if (x == 5 && y == 0) continue;
		if (x == 5 && y == 5) continue;
		std::pair<int, int> coord = std::make_pair(x, y);
		auto found = std::find(triedPairs.begin(), triedPairs.end(), coord);
		if (found != triedPairs.end()) continue;
		triedPairs.push_back(coord);

		selected = worldMap.getTerritory(x, y);
		if (selected->getTerritoryBuilding() == nullptr) continue;
		if (selected->getTerritoryBuilding()->getBuildingType() == FORT) continue;
		if (selected->getTerritoryType() != PADDY_PLAIN && selected->getTerritoryType() != COAST) continue;
		break;
	}

	if (selected == nullptr) return;
	for (auto& R : rulers)
	{
		if (R->getRulerEnum() != selected->getTerritoryRuler()) continue;
		std::cout << selected->getTerritoryBuilding()->getBuildingName() << " on " << selected->getTerritoryName() << " destroyed by flood." << std::endl;
		R->getKingdom()->destroyBuilding(selected);
	}
}
