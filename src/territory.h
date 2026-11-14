#pragma once
#include "includes.h"
#include "building.h"
#include "army.h"

class Territory
{
	private:
		std::string territoryName;
		TerrainType territoryType;
		std::vector<int> territoryBaseProduction;
		RulerEnum territoryRuler;
		std::vector<std::pair<int, int>> territoryNeighbours;
		std::unique_ptr<Building> territoryBuilding = nullptr;
	
	public:
		Territory(std::string n, TerrainType t, RulerEnum r, std::pair<int, int> location);
		
		void setTerritoryRuler(RulerEnum newRuler) { territoryRuler = newRuler; }
		
		std::string getTerritoryName() const { return territoryName; }
		TerrainType getTerritoryType() const { return territoryType; }
		RulerEnum getTerritoryRuler() const { return territoryRuler; }
		std::vector<int> getTerritoryProduction() const { return territoryBaseProduction; }
		std::vector<std::pair<int, int>> getTerritoryNeighbours() const { return territoryNeighbours; }
		std::unique_ptr<Building>& getTerritoryBuilding() { return territoryBuilding; }

		void construct(std::unique_ptr<Building>& newBuilding) { territoryBuilding = std::move(newBuilding); }
		void demolish() { territoryBuilding = nullptr; }
};
