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

class RulerFactory
{
	public:
		static std::unique_ptr<Ruler> createRulers(std::string rulerName, std::unique_ptr<Kingdom> kingdom, RulerEnum ruler)
		{
			switch (ruler)
			{
				case ANURADHAPURA:
					return std::make_unique<Conqueror>(rulerName, std::move(kingdom), ruler);
				case RUHUNA:
					return std::make_unique<AgrarianSage>(rulerName, std::move(kingdom), ruler);
				case KOTTE:
					return std::make_unique<MerchantPrince>(rulerName, std::move(kingdom), ruler);
				case KANDY:
					return std::make_unique<Strategist>(rulerName, std::move(kingdom), ruler);
				default:
					return nullptr;
			}
		}
};
