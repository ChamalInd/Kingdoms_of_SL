#pragma once
#include "includes.h"
#include "territory.h"
#include "worldmap.h"
#include "unit.h"
#include "resources.h"
#include "structs_enums.h"

// buildingCost = {rice, timber, iron, gold}

class Building
{
	protected:
		std::string buildingName;
		std::vector<TerrainType> allowedTerrains;
		Resources buildingCost;
		int upkeepCost;
		BuildingType buildingType;
		int buildingHealth = 100;
		BuildingStatus buildingStatus = OPEN;

		virtual Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const { return Resources({0, 0, 0, 0}); } 
		virtual int getStabilityIncrements() const { return 0; } 
	
	public:
		virtual ~Building() = default;

		void setBuildingStatus(BuildingStatus newStatus) { buildingStatus = newStatus; }

		std::string getBuildingName() const { return buildingName; }
		std::vector<TerrainType> getAllowedTerrains() const { return allowedTerrains; }
		Resources getBuildingCost() const { return buildingCost; }
		int getUpkeepCost() const { return upkeepCost; }
		BuildingType getBuildingType() const { return buildingType; }
		BuildingStatus getBuildingStatus() const { return buildingStatus; }
		
		BuildingEffect performEffect(Territory* parentTerritory, WorldMap& worldMap, RulerEnum ruler);
		virtual std::unique_ptr<Unit> createUnit(UnitType) {return nullptr; } 
};

class PaddyFarm : public Building
{
	public:
		PaddyFarm()
		{
			buildingName = "Paddy Farm";
			buildingType = PADDY_FARM;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE};
			buildingCost = Resources({0, 50, 0, 100});
			upkeepCost = 5;
		}
	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return Resources({30, 0, 0, 0}); } 
};

class IrrigationTank : public Building
{
	public:
		IrrigationTank()
		{
			buildingName = "Irrigation Tank";
			buildingType = IRRIGATION_LAKE;
			allowedTerrains = {LAKE};
			buildingCost = Resources({0, 100, 0, 200});
			upkeepCost = 10;
		}
	protected:	
		Resources getResourceIncrements(Territory* parentTerritory, WorldMap& map, RulerEnum ruler) const override;
};

class Mine : public Building
{
	public:
		Mine()
		{
			buildingName = "Mine";
			buildingType = MINE;
			allowedTerrains = {HILL_COUNTRY};
			buildingCost = Resources({0, 50, 0, 150});
			upkeepCost = 10;
		}
	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return Resources({0, 0, 20, 0}); } 
};

class SawMill : public Building
{
	public:
		SawMill()
		{
			buildingName = "Sawmill";
			buildingType = SAWMILL;
			allowedTerrains = {HILL_COUNTRY, JUNGLE};
			buildingCost = Resources({0, 0, 0, 100});
			upkeepCost = 5;
		}
	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return Resources({0, 30, 0, 0}); } 
};

class Market : public Building
{
	public:
		Market()
		{
			buildingName = "Market";
			buildingType = MARKET;
			allowedTerrains = {COAST, DRY_ZONE};
			buildingCost = Resources({0, 50, 0, 200});
			upkeepCost = 10;
		}
	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return Resources({0, 0, 0, 40}); } 
};

class Temple : public Building
{
	public:
		Temple()
		{
			buildingName = "Temple";
			buildingType = TEMPLE;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = Resources({0, 100, 0, 250});
			upkeepCost = 15;
		}
	protected:	
		int getStabilityIncrements() const override { return 3; } 
};

class Barracks : public Building
{
	public:
		Barracks()
		{
			buildingName = "Barracks";
			buildingType = BARRACKS;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = Resources({0, 100, 50, 200});
			upkeepCost = 10;
		}
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
		Fort()
		{
			buildingName = "Fort";
			buildingType = FORT;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = Resources({0, 150, 100, 300});
			upkeepCost = 20;
		}
	protected:	
};

class Builder
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

