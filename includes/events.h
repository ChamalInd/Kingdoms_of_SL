#pragma once
#include "includes.h"

class Event
{
	protected:
		std::string eventName;
		int noOfRounds;
		std::string eventEffect;
		std::string eventOverMsg;

	public:
		Event(std::string n, int r, std::string e, std::string m) : eventName(n), noOfRounds(r), eventEffect(e), eventOverMsg(m) {}
		virtual ~Event() = default;
		
		virtual EventType getEventType() const = 0;
		std::string getEventName() const { return eventName; }
		std::string getEventEffect() const { return eventEffect; }
		std::string getEventOverMsg() const { return eventOverMsg; }
		int getEventRounds() const { return noOfRounds; }
		
		void decreaseEventRound() { noOfRounds = std::max(0, noOfRounds - 1); }
};


class Drought : public Event
{
	public:
		Drought() : Event("Drought", 3, "Rice production of every Dry Zone and Paddy Plain territory is halved.", "Drought has ended. Rice production restored.") {}
		EventType getEventType() const override { return DROUGHT; }
};

class Flood : public Event
{
	public:
		Flood() : Event("Flood", 1, "One random Paddy Plain or Coast territory loses its building", "Flood has ended.") {}
		EventType getEventType() const override { return FLOOD; } 
};

class GoodMonsoon : public Event
{
	public:
		GoodMonsoon() : Event("Good Monsoon", 3, "Rice production of all Paddy Plain territories +50%.", "Good Monsoon has ended. Rice production restored.") {}
		EventType getEventType() const override { return GOOD_MONSOON; }
};

class Plague : public Event
{
	public:
		Plague() : Event("Plague", 2, "Every kingdom's population falls by 10% and Stability falls by 5.", "Plague has ended.") {}
		EventType getEventType() const override { return PLAGUE; }
};

class TradeCaravan : public Event
{
	public:
		TradeCaravan() : Event("Trade Caravan", 0, "The kingdom with the fewest territories receives 500 Gold.", "Trade Caravan has gone.") {}
		EventType getEventType() const override { return TRADE_CARAVAN; }
};

class RoyalFestival : public Event
{
	public:
		RoyalFestival() : Event("Royal Festival", 2, "All kingdoms gain +10 Stability once and consume 50% more Rice.", "Royal Festival has ended. Rice consumption restored.") {}
		EventType getEventType() const override { return ROYAL_FESTIVAL; }
};

class FamineIron : public Event
{
	public:
		FamineIron() : Event("Famine of Iron Age", 3, "Iron production is halved.", "Famine of Iron Age has ended. Iron production restored.") {}
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
