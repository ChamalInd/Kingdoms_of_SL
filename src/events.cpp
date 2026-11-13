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
