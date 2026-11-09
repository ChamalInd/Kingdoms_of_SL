#include "../includes/includes.h"

int main()
{
	std::cout << "Seed : " << SEED << std::endl;
	std::cout << std::endl;
	
	srand(SEED);
	SimulationManager sm;
	return 0;
}
