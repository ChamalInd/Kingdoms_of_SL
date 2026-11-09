#include "../includes/includes.h"
		
void Resources::operator+=(const std::vector<int> productionIncrement)
{
	riceCapacity += productionIncrement[0];
	timberCapacity += productionIncrement[1];
	ironCapacity += productionIncrement[2];
	goldCapacity += productionIncrement[3];
}
