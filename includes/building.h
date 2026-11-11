#pragma once
#include "includes.h"

// buildingCost = {rice, timber, iron, gold}

class Building
{
	protected:
		std::string buildingName;
		std::vector<TerrainType> allowedTerrains;
		std::vector<int> buildingCost;
		int upkeepCost;
		int buildingHealth = 100;
	
	public:
		virtual ~Building() = default;

		std::string getBuildingName() const { return buildingName; }
		std::vector<TerrainType> getAllowedTerrains() const { return allowedTerrains; }
		std::vector<int> getBuildingCost() const { return buildingCost; }
		int getUpkeepCost() const { return upkeepCost; }
		
		void performEffect();
};

class PaddyFarm : public Building
{
	public:
		PaddyFarm()
		{
			buildingName = "Paddy Farm";
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE};
			buildingCost = {0, -50, 0, -100};
			upkeepCost = 5;
		}
};

class IrrigationTank : public Building
{
	public:
		IrrigationTank()
		{
			buildingName = "Irrigation Tank";
			allowedTerrains = {LAKE};
			buildingCost = {0, -100, 0, -200};
			upkeepCost = 10;
		}
};

class Mine : public Building
{
	public:
		Mine()
		{
			buildingName = "Mine";
			allowedTerrains = {HILL_COUNTRY};
			buildingCost = {0, -50, 0, -150};
			upkeepCost = 10;
		}
};

class SawMill : public Building
{
	public:
		SawMill()
		{
			buildingName = "Sawmill";
			allowedTerrains = {HILL_COUNTRY, JUNGLE};
			buildingCost = {0, 0, 0, -100};
			upkeepCost = 5;
		}
};

class Market : public Building
{
	public:
		Market()
		{
			buildingName = "Market";
			allowedTerrains = {COAST, DRY_ZONE};
			buildingCost = {0, -50, 0, -200};
			upkeepCost = 10;
		}
};

class Temple : public Building
{
	public:
		Temple()
		{
			buildingName = "Temple";
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = {0, -100, 0, -250};
			upkeepCost = 15;
		}
};

class Barracks : public Building
{
	public:
		Barracks()
		{
			buildingName = "Barracks";
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = {0, -100, -50, -200};
			upkeepCost = 10;
		}
};

class Fort : public Building
{	
	public:
		Fort()
		{
			buildingName = "Fort";
			allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
			buildingCost = {0, -150, -100, -300};
			upkeepCost = 20;
		}
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
		}
};

