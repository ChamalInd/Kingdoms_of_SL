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
	
	public:
		Building() = default;
		virtual ~Building() = default;

		void setBuildingStatus(BuildingStatus newStatus) { buildingStatus = newStatus; }

		virtual std::string getBuildingName() const = 0; 
		virtual bool getIsAllowed(TerrainType) const = 0; 
		virtual Resources getBuildingCost() const = 0; 
		virtual int getUpkeepCost() const = 0; 
		virtual BuildingType getBuildingType() const = 0;
		
		int getBuildingHealth() const { return buildingHealth; }
		BuildingStatus getBuildingStatus() const { return buildingStatus; }
		
		virtual BuildingEffect performEffect(Territory*, WorldMap&, RulerEnum)
		{
			BuildingEffect effect;
			effect.resourceIncrement = Resources({0, 0, 0, 0});
			effect.stabilityIncrement = 0;
			return effect;
		}
};

class PaddyFarm : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE};

	public:
		std::string getBuildingName() const override { return "Paddy Farm"; } 
		Resources getBuildingCost() const override { return Resources({0, 50, 0, 100}); } 
		int getUpkeepCost() const override { return 5; } 
		BuildingType getBuildingType() const override { return PADDY_FARM; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
		BuildingEffect performEffect(Territory*, WorldMap&, RulerEnum) override;
};

class IrrigationTank : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {LAKE};

	public:
		std::string getBuildingName() const override { return "Irrigation Tank"; } 
		Resources getBuildingCost() const override { return Resources({0, 100, 0, 200}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return IRRIGATION_LAKE; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
		BuildingEffect performEffect(Territory* parentTerritory, WorldMap& worldMap, RulerEnum ruler) override;
};

class Mine : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {HILL_COUNTRY};
		
	public:
		std::string getBuildingName() const override { return "Mine"; } 
		Resources getBuildingCost() const override { return Resources({0, 50, 0, 150}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return MINE; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
		BuildingEffect performEffect(Territory*, WorldMap&, RulerEnum) override;
};

class SawMill : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {HILL_COUNTRY, JUNGLE};

	public:
		std::string getBuildingName() const override { return "Sawmill"; } 
		Resources getBuildingCost() const override { return Resources({0, 0, 0, 100}); } 
		int getUpkeepCost() const override { return 5; } 
		BuildingType getBuildingType() const override { return SAWMILL; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
		BuildingEffect performEffect(Territory*, WorldMap&, RulerEnum) override;
};

class Market : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {COAST, DRY_ZONE};

	public:
		std::string getBuildingName() const override { return "Market"; } 
		Resources getBuildingCost() const override { return Resources({0, 50, 0, 200}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return MARKET; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
		BuildingEffect performEffect(Territory*, WorldMap&, RulerEnum) override;
};

class Temple : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};

	public:
		std::string getBuildingName() const override { return "Temple"; } 
		Resources getBuildingCost() const override { return Resources({0, 100, 0, 250}); } 
		int getUpkeepCost() const override { return 15; } 
		BuildingType getBuildingType() const override { return TEMPLE; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
		BuildingEffect performEffect(Territory*, WorldMap&, RulerEnum) override;
};

class Barracks : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};

	public:
		std::string getBuildingName() const override { return "Barracks"; } 
		Resources getBuildingCost() const override { return Resources({0, 100, 50, 200}); } 
		int getUpkeepCost() const override { return 10; } 
		BuildingType getBuildingType() const override { return BARRACKS; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
};

class Fort : public Building
{	
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};

	public:
		std::string getBuildingName() const override { return "Fort"; } 
		Resources getBuildingCost() const override { return Resources({0, 150, 100, 300}); } 
		int getUpkeepCost() const override { return 20; } 
		BuildingType getBuildingType() const override { return FORT; }
		bool getIsAllowed(TerrainType terrain) const override
		{
			auto found = std::find(allowedTerrains.begin(), allowedTerrains.end(), terrain);
			return found != allowedTerrains.end();
		}
};

class BuildingFactory
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

