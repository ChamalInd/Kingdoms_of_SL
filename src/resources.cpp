#include "../includes/resources.h"
		
void Resources::operator+=(const std::vector<int> productionIncrement)
{
	riceCapacity += productionIncrement[0];
	timberCapacity += productionIncrement[1];
	ironCapacity += productionIncrement[2];
	goldCapacity += productionIncrement[3];
}

std::vector<int> Resources::getResources() const
{
	std::vector<int> resources;
	resources.push_back(goldCapacity);
	resources.push_back(riceCapacity);
	resources.push_back(timberCapacity);
	resources.push_back(ironCapacity);

	return resources;
}
