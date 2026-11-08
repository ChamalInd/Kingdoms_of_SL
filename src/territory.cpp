#include "../includes/includes.h"
	
Territory::Territory(int ID, std::string n, TerrainType t, Ruler r)
{
	territoryID = ID;
	territoryName = n;
	territoryType = t;
	territoryRuler = r;
	territoryBaseProduction = BASE_PRODUCTION[t];

}
