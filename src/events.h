#pragma once
#include "includes.h"

class Event
{
	protected:
		int noOfRounds;

	public:
		Event(int r) : noOfRounds(r) {}
		virtual ~Event() = default;
		
		virtual EventType getEventType() const = 0;
		virtual std::string getEventName() const = 0;
		virtual std::string getEventEffect() const = 0;
		virtual std::string getEventOverMsg() const = 0;
		int getEventRounds() const { return noOfRounds; }
		
		void decreaseEventRound() { noOfRounds = std::max(0, noOfRounds - 1); }
};

class Drought : public Event
{
	public:
		Drought() : Event(3) {}
		std::string getEventName() const override { return "Drought"; } 
		std::string getEventEffect() const override { return "Rice production of every Dry Zone and Paddy Plain territory is halved."; }
		std::string getEventOverMsg() const override { return "Drought has ended. Rice production restored."; }
		EventType getEventType() const override { return DROUGHT; }
};

class Flood : public Event
{
	public:
		Flood() : Event(1) {}
		std::string getEventName() const override { return "Flood"; } 
		std::string getEventEffect() const override { return "One random Paddy Plain or Coast territory loses its building."; }
		std::string getEventOverMsg() const override { return "Flood has ended."; }
		EventType getEventType() const override { return FLOOD; } 
};

class GoodMonsoon : public Event
{
	public:
		GoodMonsoon() : Event(3) {}
		std::string getEventName() const override { return "Good Monsoon"; } 
		std::string getEventEffect() const override { return "Rice production of all Paddy Plain territories +50%."; }
		std::string getEventOverMsg() const override { return "Good Monsoon has ended. Rice production restored."; }
		EventType getEventType() const override { return GOOD_MONSOON; }
};

class Plague : public Event
{
	public:
		Plague() : Event(2) {}
		std::string getEventName() const override { return "Plague"; } 
		std::string getEventEffect() const override { return "Every kingdom's population falls by 10% and Stability falls by 5."; }
		std::string getEventOverMsg() const override { return "Plague has ended."; }
		EventType getEventType() const override { return PLAGUE; }
};

class TradeCaravan : public Event
{
	public:
		TradeCaravan() : Event(0) {}
		std::string getEventName() const override { return "Trade Caravan"; } 
		std::string getEventEffect() const override { return "The kingdom with the fewest territories receives 500 Gold."; }
		std::string getEventOverMsg() const override { return "Trade Caravan has gone."; }
		EventType getEventType() const override { return TRADE_CARAVAN; }
};

class RoyalFestival : public Event
{
	public:
		RoyalFestival() : Event(2) {}
		std::string getEventName() const override { return "Royal Festival"; } 
		std::string getEventEffect() const override { return "All kingdoms gain +10 Stability once and consume 50% more Rice."; }
		std::string getEventOverMsg() const override { return "Royal Festival has ended. Rice consumption restored."; }
		EventType getEventType() const override { return ROYAL_FESTIVAL; }
};

class FamineIron : public Event
{
	public:
		FamineIron() : Event(3) {}
		std::string getEventName() const override { return "Famine of Iron Age"; } 
		std::string getEventEffect() const override { return "Iron production is halved."; }
		std::string getEventOverMsg() const override { return "Famine of Iron Age has ended. Iron production restored."; }
		EventType getEventType() const override { return F_O_IRON_AGE; }
};

class EventCreater
{
	public:
		static std::unique_ptr<Event> createEvent(EventType type)
		{
			switch (type)
			{
				case DROUGHT:
					return std::make_unique<Drought>();
				case FLOOD:
					return std::make_unique<Flood>();
				case GOOD_MONSOON:
					return std::make_unique<GoodMonsoon>();
				case PLAGUE:
					return std::make_unique<Plague>();
				case TRADE_CARAVAN:
					return std::make_unique<TradeCaravan>();
				case ROYAL_FESTIVAL:
					return std::make_unique<RoyalFestival>();
				case F_O_IRON_AGE:
					return std::make_unique<FamineIron>();
				default:
					return nullptr;
			}
		}
};
