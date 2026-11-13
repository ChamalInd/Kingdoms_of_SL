#pragma once
#include "includes.h"
#include "ruler.h"
#include "worldmap.h"
#include "kingdom.h"
#include "logger.h"

class SimulationManager
{
	private:
		WorldMap worldMap;
		Logger logger;
		std::vector<std::unique_ptr<Ruler>> rulers;
		int gameRound = 0;
		std::unique_ptr<Event> currentEvent = nullptr;
	
	public:
		SimulationManager();
		
		void initializeGame();
		void eventHandler();
		void gameLoop();
		int rollDie();
		std::vector<RulerDetails> calculateRulerDetails();
};

class RulerMaker
{
	public:
		static std::unique_ptr<Ruler> createRulers(std::string rulerName, Kingdom& kingdom, RulerEnum ruler)
		{
			switch (ruler)
			{
				case ANURADHAPURA:
					return std::make_unique<Conqueror>(rulerName, kingdom, ruler);
				case RUHUNA:
					return std::make_unique<AgrarianSage>(rulerName, kingdom, ruler);
				case KOTTE:
					return std::make_unique<MerchantPrince>(rulerName, kingdom, ruler);
				case KANDY:
					return std::make_unique<Strategist>(rulerName, kingdom, ruler);
				default:
					return nullptr;
			}
		}
};
