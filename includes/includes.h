#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <climits>
#include <iomanip>
#include <utility>

// forward declarations classes
class Resources;
class Territory;
class Building;
class Unit;
class Army;
class Kingdom;
class Ruler;
class WorldMap;
class SimulationManager;

// global variables
#define SEED            1 // time(NULL)
#define NO_OF_KINGDOMS  4
#define MAP_DIMENSIONS  6
#define NO_OF_TERRAINS  6
#define NO_OF_TILES    36
#define MESSAGE_WIDTH  70

const std::vector<std::vector<int>> BASE_PRODUCTION = {
// rice, timber, iron, gold, defense bonus
	{20,      0,    0,    0,            0},  // paddy plain
	{ 0,     10,    5,    0,           20},  // hill country
	{ 5,      0,    0,    5,            0},  // dry zone
	{ 5,      0,    0,   15,            0},  // coast
	{ 0,     20,    0,    0,           10},  // jungle
	{10,      0,    0,    5,            0}   // lake
};

// global helper functions
std::vector<int> setTheTerrainCounts();
int rollDie();
void printTitle(std::string title, char decorator);
void determinePlayerOrder(std::vector<std::unique_ptr<Ruler>>& rulers);

// enumerations 
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

enum ResourceType
{
	RICE, TIMBER, IRON, GOLD
};

enum TerrainType
{
	PADDY_PLAIN, HILL_COUNTRY, DRY_ZONE, COAST, JUNGLE, LAKE 
};

enum UnitType
{
	NONE = -1, SPEARMAN, ARCHER, CAVALRY, WAR_ELEPHANT
};
