#include "../includes/resources.h"
		
void Resources::operator+=(const Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		resourcesStock[i] += other.getResources()[i];
	}
}

void Resources::operator-=(const Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		resourcesStock[i] -= other.getResources()[i];
	}
}

bool Resources::operator>=(const Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		if (resourcesStock[i] < other.getResources()[i]) return false;
	}
	return true;
}
