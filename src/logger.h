#pragma once
#include "includes.h"
#include "ruler.h"
#include "events.h"

class Logger
{
	public:
		void printTitle(std::string title, char decorator);
		void printSimulationInitializer();
		void printSimulationEnd(const int& gameRound, std::vector<std::unique_ptr<Ruler>>& rulers);
		void printRoundSummary(std::vector<std::unique_ptr<Ruler>>& rulers, std::unique_ptr<Event>& currentEvent, const int& gameRound);
};
