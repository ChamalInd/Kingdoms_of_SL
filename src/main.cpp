#include "../includes/includes.h"
#include "../includes/simulation.h"

int main()
{
	srand(SEED);
	SimulationManager sm;
	sm.gameLoop();
	return 0;
}
