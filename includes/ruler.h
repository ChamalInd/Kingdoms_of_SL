#pragma once
#include "includes.h"
#include "kingdom.h"
#include "worldmap.h"

class Ruler
{
	protected:
		std::string rulerName;
		Kingdom kingdom;
		int firstDieValue = 0;
		bool placeChosen = false;
		int score = 0;

	public:
		Ruler(std::string n, Kingdom& k) : rulerName(n), kingdom(k) {}
		virtual ~Ruler() = default;
		
		void setFirstDieValue(int dieValue) { firstDieValue = dieValue; }
		void setPlaceChosen(bool chosen) { placeChosen = chosen; }
		
		Kingdom& getKingdom() { return kingdom; }
		int getFirstDieValue() const { return firstDieValue; }
		bool getPlaceChosen() const { return placeChosen; }
		std::string getRulerName() const { return rulerName; }
		
		void printRoundSummary();
		int calculateScore();
		virtual void playTurn(WorldMap& worldMap) = 0;
};

class Conqueror : public Ruler
{
	public:
		Conqueror(std::string n, Kingdom& k) : Ruler(n, k) {}

		void playTurn(WorldMap& worldMap) override;
};

class AgrarianSage : public Ruler
{
	public:
		AgrarianSage(std::string n, Kingdom& k) : Ruler(n, k) {}

		void playTurn(WorldMap& worldMap) override;
};

class MerchantPrince : public Ruler
{
	public:
		MerchantPrince(std::string n, Kingdom& k) : Ruler(n, k) {}

		void playTurn(WorldMap& worldMap) override;
};

class Strategist : public Ruler
{
	public:
		Strategist(std::string n, Kingdom& k) : Ruler(n, k) {}

		void playTurn(WorldMap& worldMap) override;
};
