#include "includes.h"
#include "simulation.h"

int main()
{
	srand(SEED);
	SimulationManager sm;
	sm.gameLoop();
	return 0;
}
