#pragma once
#include "includes.h"
#include "resources.h"
#include "unit.h"

class Territory;
class WorldMap;

struct BuildingEffect
{
	Resources resourceIncrement;
	int stabilityIncrement;
};

// buildingCost = {rice, timber, iron, gold}

class Building
{
	protected:
		int buildingHealth = 100;
		BuildingStatus buildingStatus = OPEN;
		
		virtual Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const { return Resources({0, 0, 0, 0}); } 
		virtual int getStabilityIncrements() const { return 0; } 
	
	public:
		Building() = default;
		virtual ~Building() = default;

		void setBuildingStatus(BuildingStatus newStatus) { buildingStatus = newStatus; }

		virtual std::string getBuildingName() const = 0; 
		virtual std::vector<TerrainType> getAllowedTerrains() const = 0; 
		virtual Resources getBuildingCost() const = 0; 
		virtual int getUpkeepCost() const = 0; 
		virtual BuildingType getBuildingType() const = 0;
		
		int getBuildingHealth() const { return buildingHealth; }
		BuildingStatus getBuildingStatus() const { return buildingStatus; }
		
		BuildingEffect performEffect(Territory* parentTerritory, WorldMap& worldMap, RulerEnum ruler);
		virtual std::unique_ptr<Unit> createUnit(UnitType) { return nullptr; } 
};

class PaddyFarm : public Building
{
	public:
		std::string getBuildingName() const override { return "Paddy Farm"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {PADDY_PLAIN, DRY_ZONE}; } 
		Resources getBuildingCost() const override { return Resources({0, 50, 0, 100}); } 
		int getUpkeepCost() const override { return 5; } 
		BuildingType getBuildingType() const override { return PADDY_FARM; }

	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override 
		{ 
			if (buildingHealth <= 50) return Resources({15, 0, 0, 0});
			return Resources({30, 0, 0, 0}); 
		} 
};

class IrrigationTank : public Building
{
	public:
		std::string getBuildingName() const override { return "Irrigation Tank"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {LAKE}; } 
		Resources getBuildingCost() const override { return Resources({0, 100, 0, 200}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return IRRIGATION_LAKE; }

	protected:	
		Resources getResourceIncrements(Territory* parentTerritory, WorldMap& map, RulerEnum ruler) const override;
};

class Mine : public Building
{
	public:
		std::string getBuildingName() const override { return "Mine"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {HILL_COUNTRY}; } 
		Resources getBuildingCost() const override { return Resources({0, 50, 0, 150}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return MINE; }

	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override 
		{
			if (buildingHealth <= 50) return Resources({0, 0, 10, 0});
			return Resources({0, 0, 20, 0}); 
		} 
};

class SawMill : public Building
{
	public:
		std::string getBuildingName() const override { return "Sawmill"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {HILL_COUNTRY, JUNGLE}; } 
		Resources getBuildingCost() const override { return Resources({0, 0, 0, 100}); } 
		int getUpkeepCost() const override { return 5; } 
		BuildingType getBuildingType() const override { return SAWMILL; }

	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override 
		{ 
			if (buildingHealth <= 50) return Resources({0, 15, 0, 0});
			return Resources({0, 30, 0, 0}); 
		} 
};

class Market : public Building
{
	public:
		std::string getBuildingName() const override { return "Market"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {COAST, DRY_ZONE}; } 
		Resources getBuildingCost() const override { return Resources({0, 50, 0, 200}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return MARKET; }

	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override 
		{ 
			if (buildingHealth <= 50) return Resources({0, 0, 0, 20}); 
			return Resources({0, 0, 0, 40}); 
		} 
};

class Temple : public Building
{
	public:
		std::string getBuildingName() const override { return "Temple"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE}; } 
		Resources getBuildingCost() const override { return Resources({0, 100, 0, 250}); } 
		int getUpkeepCost() const override { return 15; } 
		BuildingType getBuildingType() const override { return TEMPLE; }

	protected:	
		int getStabilityIncrements() const override { return 3; } 
};

class Barracks : public Building
{
	public:
		std::string getBuildingName() const override { return "Barracks"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE}; } 
		Resources getBuildingCost() const override { return Resources({0, 100, 50, 200}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return BARRACKS; }
		std::unique_ptr<Unit> createUnit(UnitType unit) override
		{
			switch (unit)
			{
				case SPEARMAN:
					return std::make_unique<Spearman>();
				case ARCHER:
					return std::make_unique<Archer>();
				case CAVALRY:
					return std::make_unique<Cavalry>();
				case WAR_ELEPHANT:
					return std::make_unique<WarElephant>();
				default:
					return nullptr;
			}
		}
};

class Fort : public Building
{	
	public:
		std::string getBuildingName() const override { return "Fort"; } 
		std::vector<TerrainType> getAllowedTerrains() const override { return {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE}; } 
		Resources getBuildingCost() const override { return Resources({0, 150, 100, 300}); } 
		int getUpkeepCost() const override { return 20; } 
		BuildingType getBuildingType() const override { return FORT; }

	protected:	
};

class BuildingMaker
{
	public:
		static std::unique_ptr<Building> createBuilding(const BuildingType& type)
		{
			switch (type)
			{
				case PADDY_FARM:
					return std::make_unique<PaddyFarm>();
				case IRRIGATION_LAKE:
					return std::make_unique<IrrigationTank>();
				case MINE:
					return std::make_unique<Mine>();
				case SAWMILL:
					return std::make_unique<SawMill>();
				case MARKET:
					return std::make_unique<Market>();
				case TEMPLE:
					return std::make_unique<Temple>();
				case BARRACKS:
					return std::make_unique<Barracks>();
				case FORT:
					return std::make_unique<Fort>();
			}
			return nullptr;
		}
};

