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
		int buildingHealth = 100;
		BuildingStatus buildingStatus = OPEN;
		
		virtual Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const { return Resources({0, 0, 0, 0}); } 
		virtual int getStabilityIncrements() const { return 0; } 
	
	public:
		Building(std::string n, std::vector<TerrainType> a, Resources r, int u) : buildingName(n), allowedTerrains(a), buildingCost(r), upkeepCost(u) {}
		virtual ~Building() = default;

		void setBuildingStatus(BuildingStatus newStatus) { buildingStatus = newStatus; }

		std::string getBuildingName() const { return buildingName; }
		std::vector<TerrainType> getAllowedTerrains() const { return allowedTerrains; }
		Resources getBuildingCost() const { return buildingCost; }
		int getUpkeepCost() const { return upkeepCost; }
		int getBuildingHealth() const { return buildingHealth; }
		BuildingStatus getBuildingStatus() const { return buildingStatus; }
		virtual BuildingType getBuildingType() const = 0;
		
		BuildingEffect performEffect(Territory* parentTerritory, WorldMap& worldMap, RulerEnum ruler);
		virtual std::unique_ptr<Unit> createUnit(UnitType) { return nullptr; } 
};

class PaddyFarm : public Building
{
	public:
		PaddyFarm() : Building("Paddy Farm", {PADDY_PLAIN, DRY_ZONE}, Resources({0, 50, 0, 100}), 5) {}
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
		IrrigationTank() : Building("Irrigation Tank", {LAKE}, Resources({0, 100, 0, 200}), 10) {}
		BuildingType getBuildingType() const override { return IRRIGATION_LAKE; }

	protected:	
		Resources getResourceIncrements(Territory* parentTerritory, WorldMap& map, RulerEnum ruler) const override;
};

class Mine : public Building
{
	public:
		Mine() : Building("Mine", {HILL_COUNTRY}, Resources({0, 50, 0, 150}), 10) {}
		BuildingType getBuildingType() const override { return MINE; }

	protected:	
		Resources getResourceIncrements(Territory*, WorldMap&, RulerEnum) const override 
		{
			if (buildingHealth <= 50) Resources({0, 0, 10, 0});
			return Resources({0, 0, 20, 0}); 
		} 
};

class SawMill : public Building
{
	public:
		SawMill() : Building("Sawmill", {HILL_COUNTRY, JUNGLE}, Resources({0, 0, 0, 100}), 5) {}
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
		Market() : Building("Market", {COAST, DRY_ZONE}, Resources({0, 50, 0, 200}), 10) {}
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
		Temple() : Building("Temple", {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE}, Resources({0, 100, 0, 250}), 15) {}
		BuildingType getBuildingType() const override { return TEMPLE; }

	protected:	
		int getStabilityIncrements() const override { return 3; } 
};

class Barracks : public Building
{
	public:
		Barracks() : Building("Barracks", {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE}, Resources({0, 100, 50, 200}), 10) {}
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
		Fort() : Building("Fort", {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE}, Resources({0, 150, 100, 300}), 20) {}
		BuildingType getBuildingType() const override { return FORT; }

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

