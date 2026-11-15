# Kingdoms of SL

**An autonomous strategy simulation in C++, set in ancient Sri Lanka.**

Four historical kingdoms, each led by an AI ruler with a distinct personality, compete on a randomly generated 6×6 map for up to 200 rounds. There is no player input. You start the program and watch four rulers expand, build, recruit, fight, starve, rebel and survive random events, until one kingdom is left standing or time runs out.

![Kingdoms of SL](assets/demo.gif)

## Highlights

- **Four AI personalities** that make different economic, building, recruiting and war decisions from the same rules.
- **Full economy loop:** four resources, population growth and starvation, taxes, building upkeep, and army upkeep.
- **Territory and combat system** with terrain defence bonuses, forts, four unit types and multi-round battles.
- **Random world events** (drought, flood, plague, and more) that shake things up every 10 rounds.
- **Stability and rebellion:** neglect your people and provinces break away.
- **Clean object-oriented design:** polymorphic rulers, units, buildings and events, created through factories.
- **Readable play-by-play log** of every claim, build, recruit, march and battle.

## Getting started

### Requirements

- A C++17 compiler (`g++` or `clang++`)
- `make`

### Build and run

```bash
git clone https://github.com/ChamalInd/Kingdoms_of_SL.git
cd Kingdoms_of_SL
make
./kingdoms
```

The simulation prints the whole game to the terminal. To save it to a file and read it later:

```bash
make save
```

Each run uses the current time as its random seed, so every game plays out differently.

> The default Makefile builds with AddressSanitizer (`-fsanitize=address`) to catch memory bugs. If you want a faster build, remove that flag from `CXXFLAGS`.

### Clean up

```bash
make clean
```

## How a game works

### Setup

1. Each kingdom gets a capital on a corner of the map, with a Barracks and a starting army.
2. Each kingdom starts with 1000 population, 70 stability and 1000 Gold, 500 Rice, 200 Timber, 100 Iron.
3. Each ruler rolls a die to decide turn order. Ties are re-rolled.

### Every round

Each surviving ruler takes a turn. Before the turn, the kingdom handles its economy: population eats rice and pays taxes, buildings and armies cost upkeep, resources are collected from territories and buildings, and stability is updated.

Then the ruler acts in four phases:

| Phase | What happens |
|---|---|
| **Claim** | Pay Gold to claim a neutral territory next to your land. |
| **Build** | Construct one building, chosen by the ruler's personality. |
| **Recruit** | Train up to 5 units at a Barracks. |
| **Move** | Move an army. Entering a neutral tile claims it. Entering an enemy tile starts a war. |

Every 10th round a random world event begins.

### Winning

- **Last kingdom standing wins.** A kingdom is eliminated when its capital is captured.
- If more than one kingdom survives 200 rounds, the highest score wins:

```
score = 20 × territories + 5 × buildings + 3 × units + gold / 50 + population / 100
```

## The kingdoms and their rulers

All four rulers follow the same rules but make different choices.

| Kingdom | Ruler | Style |
|---|---|---|
| **Anuradhapura** | The Conqueror | Aggressive. Prefers Cavalry and War Elephants, builds Forts and Barracks, spends 40% of its Gold on growth, and attacks any rival whose army is weaker than its own by a 20% margin. |
| **Ruhuna** | The Agrarian Sage | Peaceful farmer. Favours Paddy Plains, Paddy Farms and Irrigation Tanks, never attacks other kingdoms, and recruits Spearmen only when at war. |
| **Kotte** | The Merchant Prince | Trader. Favours Coast territory and Markets, recruits Archers and War Elephants, keeps a 1500 Gold reserve, and only attacks rivals at half its strength or less. |
| **Kandy** | The Strategist | Adaptive. Builds farms and temples while it is the strongest kingdom, switches to Barracks, Forts and Spearmen when it falls behind, responds to events, and only attacks when it is stronger and has more territory and stable rule. |

## The world

### Map and terrain

A 6×6 map is generated randomly each game. Each kingdom's capital sits in a corner on a Paddy Plain. Armies can move to the four neighbouring tiles (no diagonals).

| Terrain | Rice | Timber | Iron | Gold | Defence bonus |
|---|---|---|---|---|---|
| Paddy Plain | 20 | | | | |
| Hill Country | | 10 | 5 | | +20% |
| Dry Zone | 5 | | | 5 | |
| Coast | 5 | | | 15 | |
| Jungle | | 20 | | | +10% |
| Lake | 10 | | | 5 | |

### Buildings

Each territory holds one building. Buildings cost resources to build and Gold to maintain. If a kingdom cannot pay the upkeep, a building is closed until debts are cleared.

| Building | Allowed on | Cost | Upkeep | Effect |
|---|---|---|---|---|
| Paddy Farm | Paddy Plain, Dry Zone | 50 Timber, 100 Gold | 5 | +30 Rice |
| Irrigation Tank | Lake | 100 Timber, 200 Gold | 10 | +50% of neighbouring Paddy/Dry Zone rice |
| Mine | Hill Country | 50 Timber, 150 Gold | 10 | +20 Iron |
| Sawmill | Hill Country, Jungle | 100 Gold | 5 | +30 Timber |
| Market | Coast, Dry Zone | 50 Timber, 200 Gold | 10 | +40 Gold |
| Temple | Any | 100 Timber, 250 Gold | 15 | Raises stability |
| Barracks | Any | 100 Timber, 50 Iron, 200 Gold | 10 | Lets you recruit and station an army |
| Fort | Any | 150 Timber, 100 Iron, 300 Gold | 20 | +30% defence, and a boost for War Elephants stationed there |

### Units

| Unit | Attack | Defence | HP | Move | Cost | Rice upkeep | Special |
|---|---|---|---|---|---|---|---|
| Spearman | 6 | 8 | 30 | 1 | 40 Gold, 10 Iron | 2 | +25% defence bonus to its army |
| Archer | 9 | 4 | 20 | 1 | 50 Gold, 5 Iron | 2 | Cannot be killed in the first combat round |
| Cavalry | 12 | 6 | 30 | 2 | 90 Gold, 20 Iron | 4 | +20% attack in the first combat round, moves 2 tiles |
| War Elephant | 18 | 12 | 60 | 1 | 200 Gold, 40 Iron | 8 | 10% chance to deal no damage, +25% attack from a Fort |

### Combat

- An attack starts when an army moves into an enemy territory.
- Battles last up to 5 combat rounds. Both sides deal damage each round, with some randomness.
- The defender gets the territory's terrain bonus, its building bonus (such as a Fort), and the army's own defence bonuses.
- If the attacker destroys the defending army, it captures the territory (and the building on it is destroyed). If it doesn't, the attacker retreats.
- Capture a kingdom's capital and that kingdom is eliminated.

### Population, food and stability

- Population eats Rice every round and pays Gold in taxes. Plenty of rice grows the population, and a shortage shrinks it.
- Armies cost Rice upkeep. If it can't be paid, units desert.
- **Stability** goes up with surplus food and Temples, and down with starvation and lost wars. Below 30, production drops by 25%. At 0, a province rebels and is lost.

### Random events

Every 10 rounds one of these happens:

| Event | Effect |
|---|---|
| Drought | Rice production is halved for 3 rounds |
| Flood | One random Paddy Plain or Coast building is destroyed |
| Good Monsoon | Rice production +50% for 3 rounds |
| Plague | Population falls and stability drops for 2 rounds |
| Trade Caravan | The kingdom with the fewest territories receives 500 Gold |
| Royal Festival | +10 stability, but 50% more Rice is consumed for 2 rounds |
| Famine of Iron Age | Iron production is halved for 3 rounds |

## Sample output

A round summary looks like this:

```
=============================== Round 10 Summary ===============================
Kandy (The Strategist)
Gold: 25 | Rice: 75 | Timber: 380 | Iron: 0
Population: 1214 | Stability: 100
Territories: 4 | Buildings: 3 | Units: 5
--------------------------------------------------------------------------------
Ruhuna (The Agrarian Sage)
Gold: 70 | Rice: 625 | Timber: 520 | Iron: 132
Population: 1214 | Stability: 100
Territories: 5 | Buildings: 4 | Units: 0
```

And the end of a game:

```
========================== Game Over after 200 rounds ==========================
Winner : Ruhuna (The Agrarian Sage)

Final Score
1. Ruhuna       : 1283
2. Kotte        : 123
3. Kandy        : 32
4. Anuradhapura : 31
```

## Project structure

```
src/
├── main.cpp         Entry point
├── simulation.*     SimulationManager (game loop) and RulerFactory
├── ruler.*          Ruler base class and the four AI personalities
├── kingdom.*        Economy, population, stability, armies
├── territory.*      A single map tile and its owner
├── worldmap.*       Random 6×6 map generation
├── building.*       Buildings and the BuildingFactory
├── unit.*           Units and the UnitFactory
├── army.*           Groups of units, movement and combat maths
├── events.*         Random world events
├── resources.*      Resource stockpile type
├── logger.*         Console output formatting
└── includes.h       Shared constants and enums
```

## Design notes

- **Polymorphism for AI behaviour.** `Ruler` is an abstract base class. Each personality overrides how it scores buildings, territories and units, when it will attack, and how much it will spend.
- **Factories.** `RulerFactory`, `BuildingFactory` and `UnitFactory` create objects, and `std::unique_ptr` owns them, so there is no manual memory management.
- **Data-driven balance.** Terrain output, costs and upkeep live in a few tables, so tuning the game means changing numbers.
- **Deterministic debugging.** The random seed is set in one place (`SEED` in `includes.h`). Set it to a fixed number to replay the same game.
