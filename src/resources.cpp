#include "../includes/resources.h"
		
void Resources::operator+=(const Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		resourcesStock[i] += other[i];
	}
}

void Resources::operator-=(const Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		resourcesStock[i] -= other[i];
	}
}

bool Resources::operator>=(const Resources& other) const
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		if (resourcesStock[i] < other[i]) return false;
	}
	return true;
}
