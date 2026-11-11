#pragma once
#include "includes.h"
#include "territory.h"
#include "worldmap.h"

// buildingCost = {rice, timber, iron, gold}

class Building
{
	protected:
		std::string buildingName;
		std::vector<TerrainType> allowedTerrains;
		std::vector<int> buildingCost;
		int upkeepCost;
		BuildingType buildingType;
		int buildingHealth = 100;
		BuildingStatus buildingStatus = OPEN;

		virtual std::vector<int> getResourceIncrements(Territory*, WorldMap&, RulerEnum) const { return {0, 0, 0, 0}; } 
		virtual int getStabilityIncrements() const { return 0; } 
	
	public:
		virtual ~Building() = default;

		void setBuildingStatus(BuildingStatus newStatus) { buildingStatus = newStatus; }

		std::string getBuildingName() const { return buildingName; }
		std::vector<TerrainType> getAllowedTerrains() const { return allowedTerrains; }
		std::vector<int> getBuildingCost() const { return buildingCost; }
		int getUpkeepCost() const { return upkeepCost; }
		BuildingType getBuildingType() const { return buildingType; }
		BuildingStatus getBuildingStatus() const { return buildingStatus; }
		
		BuildingEffect performEffect(Territory* parentTerritory, WorldMap& worldMap, RulerEnum ruler);
};

class PaddyFarm : public Building
{
	public:
		PaddyFarm()
		{
			buildingName = "Paddy Farm";
			buildingType = PADDY_FARM;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE};
			buildingCost = {0, -50, 0, -100};
			upkeepCost = 5;
		}
	protected:	
		std::vector<int> getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return {30, 0, 0, 0}; } 
};

class IrrigationTank : public Building
{
	public:
		IrrigationTank()
		{
			buildingName = "Irrigation Tank";
			buildingType = IRRIGATION_LAKE;
			allowedTerrains = {LAKE};
			buildingCost = {0, -100, 0, -200};
			upkeepCost = 10;
		}
	protected:	
		std::vector<int> getResourceIncrements(Territory* parentTerritory, WorldMap& map, RulerEnum ruler) const override;
};

class Mine : public Building
{
	public:
		Mine()
		{
			buildingName = "Mine";
			buildingType = MINE;
			allowedTerrains = {HILL_COUNTRY};
			buildingCost = {0, -50, 0, -150};
			upkeepCost = 10;
		}
	protected:	
		std::vector<int> getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return {0, 0, 20, 0}; } 
};

class SawMill : public Building
{
	public:
		SawMill()
		{
			buildingName = "Sawmill";
			buildingType = SAWMILL;
			allowedTerrains = {HILL_COUNTRY, JUNGLE};
			buildingCost = {0, 0, 0, -100};
			upkeepCost = 5;
		}
	protected:	
		std::vector<int> getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return {0, 30, 0, 0}; } 
};

class Market : public Building
{
	public:
		Market()
		{
			buildingName = "Market";
			buildingType = MARKET;
			allowedTerrains = {COAST, DRY_ZONE};
			buildingCost = {0, -50, 0, -200};
			upkeepCost = 10;
		}
	protected:	
		std::vector<int> getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override { return {0, 0, 0, 40}; } 
};

class Temple : public Building
{
	public:
		Temple()
		{
			buildingName = "Temple";
			buildingType = TEMPLE;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = {0, -100, 0, -250};
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
			buildingCost = {0, -100, -50, -200};
			upkeepCost = 10;
		}
	protected:	
};

class Fort : public Building
{	
	public:
		Fort()
		{
			buildingName = "Fort";
			buildingType = FORT;
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = {0, -150, -100, -300};
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

