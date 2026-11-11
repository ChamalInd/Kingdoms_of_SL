#pragma once
#include "includes.h"
#include "kingdom.h"
#include "worldmap.h"

class Ruler
{
	protected:
		std::string rulerName;
		Kingdom kingdom;
		RulerEnum rulerEnum;
		int firstDieValue = 0;
		bool placeChosen = false;
		int score = 0;

		virtual int claimBudget() const = 0;
		virtual int scoreTerritory(const Territory*) const { return 0; }
		virtual int scoreBuilding(const BuildingType) const { return 0; }

	private:
		void claimTerritory(Territory* territory, int claimCost);
		void constructBuilding(Territory* territory, std::unique_ptr<Building>& building);
		void claimPhase(WorldMap& worldMap);
		void buildPhase();

	public:
		Ruler(std::string n, Kingdom& k, RulerEnum r) : rulerName(n), kingdom(k), rulerEnum(r) {}
		virtual ~Ruler() = default;
		
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		
		Kingdom& getKingdom() { return kingdom; }
		int getFirstDieValue() const { return firstDieValue; }
		bool getPlaceChosen() const { return placeChosen; }
		std::string getRulerName() const { return rulerName; }
		RulerEnum getRulerEnum() const { return rulerEnum; }
		
		void printRoundSummary();
		int calculateScore();
		void playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails);
		std::vector<Territory*> checkNeighbouringTerritory(WorldMap& worldMap);
};

class Conqueror : public Ruler
{
	public:
		Conqueror(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}
	
	protected:
		int claimBudget() const override { return kingdom.getResources()[3] * 40 / 100; }
		int scoreBuilding(const BuildingType type) const override 
		{
			if (type == BARRACKS && kingdom.getNoOfTerritoriesOwned() % 2 == 0 && kingdom.getSpecificBuildingCount(type) < 4) return 10;
			else if (type == TEMPLE) return 0;
			else return 5;
		}
};

class AgrarianSage : public Ruler
{
	public:
		AgrarianSage(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}
	
	protected:
		int claimBudget() const override { return kingdom.getResources()[3]; }
		int scoreTerritory(const Territory* T) const override { return T->getTerritoryType() == PADDY_PLAIN ? 10 : 0; }
		int scoreBuilding(const BuildingType type) const override 
		{
			if (type == PADDY_FARM) return 10; 
			else if (type == IRRIGATION_LAKE) return 9;
			else if (type == TEMPLE && kingdom.getNoOfTerritoriesOwned() % 4 == 0) return 8;
			else return 0;
		}
};

class MerchantPrince : public Ruler
{
	public:
		MerchantPrince(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}

	protected:
		int claimBudget() const override { return kingdom.getResources()[3] - 1500; }
		int scoreTerritory(const Territory* T) const override { return T->getTerritoryType() == COAST ? 10 : 0; }
		int scoreBuilding(const BuildingType type) const override 
		{
			if (type == MARKET) return 10; 
			else return 0;
		}

};

class Strategist : public Ruler
{
	public:
		Strategist(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}

	protected:
		int claimBudget() const override { return kingdom.getResources()[3]; }
		int scoreBuilding(const BuildingType type) const override 
		{
			if (type == PADDY_FARM) return 10;
			else if (type == TEMPLE) return 9;
			else return 0;
		}
};
