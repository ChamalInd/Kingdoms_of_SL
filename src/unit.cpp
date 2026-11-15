#include "unit.h"
		
int Cavalry::getAttack(Territory*, int turn) const 
{
	if (turn == 1) return attack * 120 / 100;
	return attack;
}

int WarElephant::getAttack(Territory* territory, int) const 
{
	int r = rand() % 10;
	if (r == 1) return 0;
	if (territory->getTerritoryBuilding() != nullptr && territory->getTerritoryBuilding()->getBuildingType() == FORT) return attack * 125 / 100;
	return attack;
}
