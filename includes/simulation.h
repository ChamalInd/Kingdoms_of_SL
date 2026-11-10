#pragma once
#include "includes.h"
#include "ruler.h"
#include "worldmap.h"

class SimulationManager
{
	private:
		WorldMap worldMap;
		std::vector<std::unique_ptr<Ruler>> rulers;
		int gameRound = 0;
	
	public:
		SimulationManager();
		
		void initializeGame();
		void gameLoop();
		void endGame();
		std::vector<RulerDetails> calculateRulerDetails();
};
