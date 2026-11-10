#pragma once
#include "includes.h"

// buildingCost = {rice, timber, iron, gold}

class Building
{
	protected:
		std::vector<TerrainType> allowedTerrains;
		std::vector<int> buildingCost;
		int upkeepCost;
		int buildingHealth = 100;

	public:
		virtual ~Building() = default;

		std::vector<int> getBuildingCost() const { return buildingCost; }
		int getUpkeepCost() const { return upkeepCost; }
		
		virtual void performEffect() = 0;
};

class PaddyFarm : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE};
		std::vector<int> buildingCost = {0, -50, 0, -100};
		int upkeepCost = 5;
	
	public:
		void performEffect() override;
};

class IrrigationTank : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {LAKE};
		std::vector<int> buildingCost = {0, -100, 0, -200};
		int upkeepCost = 10;
	
	public:
		void performEffect() override;
};

class Mine : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {HILL_COUNTRY};
		std::vector<int> buildingCost = {0, -50, 0, -150};
		int upkeepCost = 10;
	
	public:
		void performEffect() override;
};

class SawMill : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {HILL_COUNTRY, JUNGLE};
		std::vector<int> buildingCost = {0, 0, 0, -100};
		int upkeepCost = 5;
	
	public:
		void performEffect() override;
};

class Market : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {COAST, DRY_ZONE};
		std::vector<int> buildingCost = {0, -50, 0, -200};
		int upkeepCost = 10;
	
	public:
		void performEffect() override;
};

class Temple : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
		std::vector<int> buildingCost = {0, -100, 0, -250};
		int upkeepCost = 15;
	
	public:
		void performEffect() override;
};

class Barracks : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
		std::vector<int> buildingCost = {0, -100, -50, -200};
		int upkeepCost = 10;
	
	public:
		void performEffect() override;
};

class Fort : public Building
{
	private:
		std::vector<TerrainType> allowedTerrains = {PADDY_PLAIN, DRY_ZONE, COAST, JUNGLE, HILL_COUNTRY, LAKE};
		std::vector<int> buildingCost = {0, -150, -100, -300};
		int upkeepCost = 20;
	
	public:
		void performEffect() override;
};
