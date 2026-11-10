#include "../includes/resources.h"
		
void Resources::operator+=(const std::vector<int> productionIncrement)
{
	riceStock += productionIncrement[0];
	timberStock += productionIncrement[1];
	ironStock += productionIncrement[2];
	goldStock += productionIncrement[3];
}

std::vector<int> Resources::getResources() const
{
	std::vector<int> resources;
	resources.push_back(riceStock);
	resources.push_back(timberStock);
	resources.push_back(ironStock);
	resources.push_back(goldStock);

	return resources;
}
