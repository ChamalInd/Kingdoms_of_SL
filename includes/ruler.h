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
		virtual int scoreTerritory(const Territory* T) const { return 0; }

	private:
		void claimPhase(WorldMap& worldMap);

	public:
		Ruler(std::string n, Kingdom& k, RulerEnum r) : rulerName(n), kingdom(k), rulerEnum(r) {}
		virtual ~Ruler() = default;
		
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		
		Kingdom& getKingdom() { return kingdom; }
		int getFirstDieValue() const { return firstDieValue; }
		bool getPlaceChosen() const { return placeChosen; }
		std::string getRulerName() const { return rulerName; }
		
		void printRoundSummary();
		int calculateScore();
		void playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails);
		std::vector<Territory*> checkNeighbouringTerritory(WorldMap& worldMap);
		int calculateTheTerritoryClaimCost();
		void claimTerritory(Territory* territory);
};

class Conqueror : public Ruler
{
	public:
		Conqueror(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}
	
	protected:
		int claimBudget() const override { return kingdom.getResources()[3] * 40 / 100; }
};

class AgrarianSage : public Ruler
{
	public:
		AgrarianSage(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}
	
	protected:
		int claimBudget() const override { return kingdom.getResources()[3]; }
		int scoreTerritory(const Territory* T) const override { return T->getTerritoryType() == PADDY_PLAIN ? 10 : 0; }
};

class MerchantPrince : public Ruler
{
	public:
		MerchantPrince(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}

	protected:
		int claimBudget() const override { return kingdom.getResources()[3] - 1500; }
		int scoreTerritory(const Territory* T) const override { return T->getTerritoryType() == COAST ? 10 : 0; }

};

class Strategist : public Ruler
{
	public:
		Strategist(std::string n, Kingdom& k, RulerEnum r) : Ruler(n, k, r) {}

	protected:
		int claimBudget() const override { return kingdom.getResources()[3]; }
};
