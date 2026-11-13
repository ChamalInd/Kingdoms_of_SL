#include "events.h"


ResourceEffect Event::applyResourceEffect() const 
{
	ResourceEffect effect; 
	effect.resource = RICE;
	effect.scale = 1;
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
	effect.populationScale = 1;
	effect.consumptionScale = 1;
	return effect;
}

PopulationEffect Plague::applyPopulationEffect() const
{
	PopulationEffect effect;
	effect.populationScale = 90;
	effect.consumptionScale = 1;
	return effect;
}

PopulationEffect RoyalFestival::applyPopulationEffect() const
{
	PopulationEffect effect;
	effect.populationScale = 1;
	effect.consumptionScale = 150;
	return effect;
}
