#pragma once

// external libraries used
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstring>
#include <vector>

// enumerations
enum TerrainType
{
	PADDY_PLAIN, HILL_COUNTRY, DRY_ZONE, COAST, JUNGLE, LAKE 
};

enum Ruler
{
	NEUTRAL = -1, ANURADHAPURA, RUHUNA, KOTTE, KANDY
};

// global variables
#define SEED 1

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
class Kingdom
{
	private:
		std::string kingdomName;
		int riceCapacity = 500;
		int timberCapacity = 200;
		int ironCapacity = 100;
		int goldCapacity = 1000;
		int population = 1000;
		int stability = 70;
		std::vector<std::pair<int, int>> territoryLocations;
		// armies = no army
		// barracks = 1 at capital
	
	public:
		Kingdom(std::string n);
};

class Territory
{
	private:
		int territoryID;
		std::string territoryName;
		TerrainType territoryType;
		std::vector<int> territoryBaseProduction;
		Ruler territoryRuler;
		// Building territoryBuilding;
		// Army
	
	public:
		Territory(int ID, std::string n, TerrainType, Ruler r);
};

class SimulationManager
{
	private:
		std::vector<std::vector<Territory>> worldMap;
		std::vector<Kingdom> kingdoms;
	
	public:
		SimulationManager();
};
