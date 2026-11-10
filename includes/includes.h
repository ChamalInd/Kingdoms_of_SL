#pragma once

// external libraries used
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <utility>

// enumerations
enum TerrainType
{
	PADDY_PLAIN, HILL_COUNTRY, DRY_ZONE, COAST, JUNGLE, LAKE 
};

enum RulerEnum
{
	NEUTRAL = -1, ANURADHAPURA, RUHUNA, KOTTE, KANDY
};

enum RollType
{
	VALUE, DOUBLES
};

// global variables
#define SEED   time(NULL)
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

// class declarations
class Resources;
class Territory;
class Kingdom;
class Ruler;
class SimulationManager;

// global helper functions
std::vector<int> setTheTerrainCounts();
int rollDie(RollType type);
void printTitle(std::string title, char decorator);
void determinePlayerOrder(std::vector<std::unique_ptr<Ruler>>& rulers);
