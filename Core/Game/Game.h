#pragma once
#include <string>
#include <vector>

#include <Game/GameEvent.h>
#include <Game/GameView.h>
#include <Game/Random.h>
#include <Game/Reputation.h>
#include <Model/Painting.h>
#include <Model/RoundType.h>


namespace lhs::game
{
	// The paintings a game deals from.
	struct Catalog
	{
		std::vector<model::Painting> paintings;
		std::vector<std::string> painters;
	};

	// How long each phase lasts, in seconds. The defaults are the lengths of the current scenes.
	struct Timings
	{
		float roundIntro = 6.f;	// three lines of the round explanation, 2 seconds each
	};

	/**
	* A game from dealing the galleries to the ranking
	*
	* It takes commands from the players and the time passed through Update,
	* and reports its progress as events.
	*/
	class Game
	{
	public:
		Game(Catalog catalog, std::vector<std::string> playerNames, Random random, Timings timings = {});

		// Deal the galleries and begin the first round. A finished game starts over.
		void Start();

		// Let the time pass by dt seconds.
		void Update(float dt);

		const GameView& View() const noexcept;

		// Return the events since the last call, in the order they happened.
		[[nodiscard]] std::vector<GameEvent> TakeEvents();

	private:
		void Enter(Phase phase, float duration = 0.f);

		void OnTimeOut();

	private:
		static constexpr int PAINTINGS_PER_PLAYER = 10;

		Catalog catalog;

		std::vector<std::string> playerNames;

		Random random;

		Timings timings;

		GameView view;

		std::vector<model::RoundType> rounds;	// the round types in the order they are played

		Reputation reputation;

		std::vector<GameEvent> events;	// the events not taken yet
	};
}
