#pragma once

// external libraries used
#include <iostream>
#include <string>
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

enum Ruler
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
class Resources
{
	private:
		int riceCapacity = 500;
		int timberCapacity = 200;
		int ironCapacity = 100;
		int goldCapacity = 1000;
};

class Kingdom
{
	private:
		std::string kingdomName;
		int firstDieValue = 0;
		bool placeChosen = false;
		int population = 1000;
		int stability = 70;
		Resources resources;
		std::vector<std::pair<int, int>> territoryLocations;
	
	public:
		Kingdom(std::string n);
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		void setCapitalCity(std::pair<int, int> capital) { territoryLocations.push_back(capital); }
		int getFirstDieValue() const { return firstDieValue; }
		std::string getKingdomName() const { return kingdomName; }
		bool getPlaceChosen() const { return placeChosen; }
};

class Territory
{
	private:
		int territoryID;
		std::string territoryName;
		TerrainType territoryType;
		std::vector<int> territoryBaseProduction;
		Ruler territoryRuler;
		std::vector<std::pair<int, int>> territoryNeighbours;
	
	public:
		Territory(int ID, std::string n, TerrainType, Ruler r, std::pair<int, int>);
		std::string getTerritoryName() const { return territoryName; }
		TerrainType getTerritoryType() const { return territoryType; }
		Ruler getTerritoryRuler() const { return territoryRuler; }
};

class WorldMap
{
	private:
		std::vector<std::vector<Territory>> worldMap{MAP_DIMENSIONS};
	
	public:
		WorldMap();
		void printWorldMap();
		Territory& getATerretory(int i, int j) { return worldMap[i][j]; };
};

class SimulationManager
{
	private:
		std::vector<Kingdom> kingdoms;
		WorldMap worldMap;
		int gameRound = 0;
	
	public:
		SimulationManager();
		void initializeGame();
};

// global helper functions
std::vector<int> setTheTerrainCounts();
int rollDie(RollType type);
void determinePlayerOrder(std::vector<Kingdom>& kingdoms);
