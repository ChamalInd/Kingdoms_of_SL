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
class Resources
{
	private:
		int riceCapacity = 500;
		int timberCapacity = 200;
		int ironCapacity = 100;
		int goldCapacity = 1000;
	
	public:
		void operator+=(const std::vector<int> productionIncrement);
		std::vector<int> getResources() const;
};

class Territory
{
	private:
		int territoryID;
		std::string territoryName;
		TerrainType territoryType;
		std::vector<int> territoryBaseProduction;
		RulerEnum territoryRuler;
		std::vector<std::pair<int, int>> territoryNeighbours;
	
	public:
		Territory(int ID, std::string n, TerrainType, RulerEnum r, std::pair<int, int>);
		std::string getTerritoryName() const { return territoryName; }
		TerrainType getTerritoryType() const { return territoryType; }
		RulerEnum getTerritoryRuler() const { return territoryRuler; }
		std::vector<int> getTerritoryProduction() const { return territoryBaseProduction; }
};

class WorldMap
{
	private:
		std::vector<std::vector<Territory>> worldMap{MAP_DIMENSIONS};
	
	public:
		WorldMap();
		Territory* getTerritory(int i, int j) { return &worldMap[i][j]; };
		void printWorldMap();
};

class Kingdom
{
	private:
		std::string kingdomName;
		int population = 1000;
		int stability = 70;
		Resources resources;
		std::vector<Territory*> ownedTerritories;
	
	public:
		Kingdom(std::string n);
		void setCapitalCity(Territory* capital) { ownedTerritories.push_back(capital); }
		std::string getKingdomName() const { return kingdomName; }
		int getPopulation() const { return population; }
		int getStability() const { return stability; }
		int getNoOfTerritoriesOwned() const { return ownedTerritories.size(); }
		std::vector<int> getResources() const { return resources.getResources(); }
		void collectProductions();
};

class Ruler
{
	private:
		std::string rulerName;
		Kingdom kingdom;
		int firstDieValue = 0;
		bool placeChosen = false;
		int score = 0;

	public:
		Ruler(std::string n, Kingdom& k) : rulerName(n), kingdom(k) {}
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		Kingdom& getKingdom() { return kingdom; }
		int getFirstDieValue() const { return firstDieValue; }
		bool getPlaceChosen() const { return placeChosen; }
		std::string getRulerName() const { return rulerName; }
		void printRoundSummary();
		int calculateScore();
};

class SimulationManager
{
	private:
		WorldMap worldMap;
		std::vector<Ruler> rulers;
		int gameRound = 0;
	
	public:
		SimulationManager();
		~SimulationManager();
		void initializeGame();
		void gameLoop();
};

// global helper functions
std::vector<int> setTheTerrainCounts();
int rollDie(RollType type);
void printTitle(std::string title, char decorator);
void determinePlayerOrder(std::vector<Ruler>& rulers);
