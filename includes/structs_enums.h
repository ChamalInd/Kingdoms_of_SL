#pragma once
#include "includes.h"
#include "resources.h"

// enumerations
enum TerrainType
{
	PADDY_PLAIN, HILL_COUNTRY, DRY_ZONE, COAST, JUNGLE, LAKE 
};

enum RulerEnum
{
	NEUTRAL = -1, ANURADHAPURA, RUHUNA, KOTTE, KANDY
};

enum BuildingType 
{
	PADDY_FARM, IRRIGATION_LAKE, MINE, SAWMILL, MARKET, TEMPLE, BARRACKS, FORT
};

enum BuildingStatus
{
	OPEN, CLOSE
};

enum UnitType
{
	NONE = -1, SPEARMAN, ARCHER, CAVALRY, WAR_ELEPHANT
};

// structures
struct RulerDetails
{
	int noOfTerritoriesOwn;
	int armyStrength;
};

struct BuildingEffect
{
	Resources resourceIncrement;
	int stabilityIncrement;
};
