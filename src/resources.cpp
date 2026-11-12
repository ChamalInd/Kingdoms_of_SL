#include "../includes/resources.h"
		
void Resources::operator+=(Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		resourcesStock[i] += other.getResources()[i];
	}
}

void Resources::operator-=(Resources& other)
{
	for (size_t i = 0; i < resourcesStock.size(); i++)
	{
		resourcesStock[i] -= other.getResources()[i];
	}
}

bool Resources::operator<(Resources& other)
{
	for (int i = 0; i < 0; i++)
	{
		if (resourcesStock[i] > other.getResources()[i]) return false;
	}
	return true;
}

bool Resources::operator>(Resources& other)
{
	for (int i = 0; i < 0; i++)
	{
		if (resourcesStock[i] < other.getResources()[i]) return false;
	}
	return true;
}
