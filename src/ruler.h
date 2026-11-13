#pragma once
#include "includes.h"
#include "kingdom.h"
#include "worldmap.h"
#include "building.h"
#include "unit.h"

struct RulerDetails
{
	int noOfTerritoriesOwn;
	int armyStrength;
};

class Ruler
{
	protected:
		std::string rulerName;
		std::unique_ptr<Kingdom> kingdom = nullptr;
		RulerEnum rulerEnum;
		int firstDieValue = 0;
		bool placeChosen = false;
		int score = 0;

		virtual int claimBudget() const = 0;
		virtual int scoreTerritory(const Territory*) const { return 0; }
		virtual int scoreBuilding(const BuildingType) const { return 0; }
		virtual int scoreUnits(const UnitType) const { return 0; }

	private:
		void recruiteUnits(Territory* territory, std::vector<std::unique_ptr<Unit>>& units, Resources totalRecruitementCost);
		void claimPhase(WorldMap& worldMap);
		void buildPhase();
		void recruitePhase();

	public:
		Ruler(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : rulerName(n), kingdom(std::move(k)), rulerEnum(r) {}
		virtual ~Ruler() = default;
		
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		
		std::unique_ptr<Kingdom>& getKingdom() { return kingdom; }
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
		Conqueror(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}
	
	protected:
		int claimBudget() const override;
		int scoreBuilding(const BuildingType type) const override; 
		int scoreUnits(const UnitType unit) const override;
};

class AgrarianSage : public Ruler
{
	public:
		AgrarianSage(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}
	
	protected:
		int claimBudget() const override;
		int scoreTerritory(const Territory* T) const override;
		int scoreBuilding(const BuildingType type) const override;
		int scoreUnits(const UnitType unit) const override;
};

class MerchantPrince : public Ruler
{
	public:
		MerchantPrince(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}

	protected:
		int claimBudget() const override;
		int scoreTerritory(const Territory* T) const override;
		int scoreBuilding(const BuildingType type) const override;
		int scoreUnits(const UnitType unit) const override;
};

class Strategist : public Ruler
{
	public:
		Strategist(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}

	protected:
		int claimBudget() const override;
		int scoreBuilding(const BuildingType type) const override;
		int scoreUnits(const UnitType unit) const override;
};
