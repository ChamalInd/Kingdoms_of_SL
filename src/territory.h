#pragma once
#include "includes.h"
#include "building.h"
#include "army.h"

class Territory
{
	private:
		int territoryID;
		std::string territoryName;
		TerrainType territoryType;
		std::vector<int> territoryBaseProduction;
		RulerEnum territoryRuler;
		std::vector<std::pair<int, int>> territoryNeighbours;
		std::unique_ptr<Building> territoryBuilding = nullptr;
		std::unique_ptr<Army> territoryArmy = nullptr;
	
	public:
		Territory(int ID, std::string n, TerrainType, RulerEnum r, std::pair<int, int>);
		
		void setTerritoryRuler(RulerEnum newRuler) { territoryRuler = newRuler; }
		
		std::string getTerritoryName() const { return territoryName; }
		TerrainType getTerritoryType() const { return territoryType; }
		RulerEnum getTerritoryRuler() const { return territoryRuler; }
		std::vector<int> getTerritoryProduction() const { return territoryBaseProduction; }
		std::vector<std::pair<int, int>> getTerritoryNeighbours() const { return territoryNeighbours; }
		std::unique_ptr<Building>& getTerritoryBuilding() { return territoryBuilding; }
		std::unique_ptr<Army>& getTerritoryArmy() { return territoryArmy; }

		void construct(std::unique_ptr<Building>& newBuilding) { territoryBuilding = std::move(newBuilding); }
		void addAnArmy(std::unique_ptr<Army> newArmy) { territoryArmy = std::move(newArmy); }
		void demolish() { territoryBuilding = nullptr; }
};
