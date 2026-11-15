#pragma once
#include "includes.h"
#include "logger.h"
#include "kingdom.h"
#include "worldmap.h"
#include "building.h"
#include "unit.h"

struct RulerDetails
{
	RulerEnum ruler;
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
		bool eliminated = false;
		int eliminatedAt = 0;
		KingdomStatus kingdomState = PEACE;

		virtual int claimBudget() const = 0;
		virtual Resources constructionBudget(std::unique_ptr<Event>&) const = 0;
		virtual Resources recruiteBudget() const = 0;
		virtual int scoreTerritory(const Territory*) const { return 0; }
		virtual int scoreBuilding(const BuildingType, std::vector<RulerDetails>, std::unique_ptr<Event>&) const = 0;
		virtual int scoreUnits(const UnitType, std::vector<RulerDetails>) const = 0;
		virtual bool hasPermissionToMove(Territory*, std::vector<RulerDetails>) const = 0;

	private:
		void claimPhase(WorldMap& worldMap, Territory* territory);
		void buildPhase(std::unique_ptr<Event>& currentEvent, std::vector<RulerDetails> rd);
		void recruitePhase(std::vector<RulerDetails> rd);
		void movePhase(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails, std::vector<std::unique_ptr<Ruler>>& rulers, int gameRound);
		void initiateWar(Ruler* attacker, Ruler* defender, Territory* territory, int attackArmy, int gameRound);
		void attackerWon(Ruler* winner, Ruler* loser, Territory* territory, int gameRound);
		void defenderWon(Ruler* winner, Ruler* loser, int attackArmy);

	public:
		Ruler(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : rulerName(n), kingdom(std::move(k)), rulerEnum(r) {}
		virtual ~Ruler() = default;
		
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		void setKingdomState(KingdomStatus newState) { kingdomState = newState; }
		
		std::unique_ptr<Kingdom>& getKingdom() { return kingdom; }
		int getFirstDieValue() const { return firstDieValue; }
		bool getPlaceChosen() const { return placeChosen; }
		std::string getRulerName() const { return rulerName; }
		RulerEnum getRulerEnum() const { return rulerEnum; }
		bool getEliminated() const { return eliminated; }
		
		void printRoundSummary();
		int calculateScore();
		void eliminateRuler(int gameRound);
		void playTurn(WorldMap& worldMap, std::vector<RulerDetails> rulerDetails, std::unique_ptr<Event>& currentEvent, std::vector<std::unique_ptr<Ruler>>& rulers, int gameRound);
		std::vector<Territory*> checkNeighbouringTerritory(WorldMap& worldMap);
};

class Conqueror : public Ruler
{
	public:
		Conqueror(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}
	
	protected:
		int claimBudget() const override;
		Resources constructionBudget(std::unique_ptr<Event>&) const override;
		Resources recruiteBudget() const override;
		int scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const override; 
		int scoreUnits(const UnitType unit, std::vector<RulerDetails>) const override;
		bool hasPermissionToMove(Territory* territory, std::vector<RulerDetails> rulerDetails) const override;
};

class AgrarianSage : public Ruler
{
	public:
		AgrarianSage(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}
	
	protected:
		int claimBudget() const override;
		Resources constructionBudget(std::unique_ptr<Event>&) const override;
		Resources recruiteBudget() const override;
		int scoreTerritory(const Territory* T) const override;
		int scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const override;
		int scoreUnits(const UnitType unit, std::vector<RulerDetails>) const override;
		bool hasPermissionToMove(Territory* territory, std::vector<RulerDetails>) const override;
};

class MerchantPrince : public Ruler
{
	public:
		MerchantPrince(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}

	protected:
		int claimBudget() const override;
		Resources constructionBudget(std::unique_ptr<Event>& currentEvent) const override;
		Resources recruiteBudget() const override;
		int scoreTerritory(const Territory* T) const override;
		int scoreBuilding(const BuildingType type, std::vector<RulerDetails>, std::unique_ptr<Event>&) const override;
		int scoreUnits(const UnitType unit, std::vector<RulerDetails>) const override;
		bool hasPermissionToMove(Territory* territory, std::vector<RulerDetails> rulerDetails) const override;
};

class Strategist : public Ruler
{
	public:
		Strategist(std::string n, std::unique_ptr<Kingdom> k, RulerEnum r) : Ruler(n, std::move(k), r) {}

	protected:
		int claimBudget() const override;
		Resources constructionBudget(std::unique_ptr<Event>&) const override;
		Resources recruiteBudget() const override;
		int scoreBuilding(const BuildingType type, std::vector<RulerDetails> rd, std::unique_ptr<Event>& currentEvent) const override;
		int scoreUnits(const UnitType unit, std::vector<RulerDetails> rd) const override;
		bool hasPermissionToMove(Territory* territory, std::vector<RulerDetails> rulerDetails) const override;
};
