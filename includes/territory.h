#pragma once
#include "includes.h"

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
		
		void setTerritoryRuler(RulerEnum newRuler) { territoryRuler = newRuler; }
		
		std::string getTerritoryName() const { return territoryName; }
		TerrainType getTerritoryType() const { return territoryType; }
		RulerEnum getTerritoryRuler() const { return territoryRuler; }
		std::vector<int> getTerritoryProduction() const { return territoryBaseProduction; }
};
