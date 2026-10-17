#pragma once
#include <string>

#include <Game/Game.h>


// Games set up for the tests
namespace lhs::test
{
	using namespace lhs::game;
	using namespace lhs::model;

	// 5 painters with 10 paintings each, like the game data
	inline Catalog MakeCatalog()
	{
		Catalog catalog;

		for (int p = 0; p < 5; ++p)
		{
			auto painter = "Painter " + std::to_string(p);
			catalog.painters.push_back(painter);

			for (int i = 0; i < 10; ++i)
				catalog.paintings.push_back({ {}, painter + " #" + std::to_string(i), painter });
		}
		return catalog;
	}

	inline Game MakeGame(uint32_t seed = 1)
	{
		return Game{ MakeCatalog(), { "You", "Andrew", "Benjamin", "Catherine" }, Random{ seed } };
	}

	inline Painting FirstPainting(const Game& game, PlayerId player)
	{
		return game.View().players.at(player).GetPaintings().front();
	}

	// Start a game whose first round is of the given type.
	inline Game MakeGameOfRound(RoundType type, uint32_t seed = 1)
	{
		for (;; ++seed)
		{
			auto game = MakeGame(seed);
			game.Start();

			if (game.View().roundType == type)
				return game;
		}
	}

	// Play to the bidding of the first auction.
	inline Game MakeGameInBidding(RoundType type, uint32_t seed = 1)
	{
		auto game = MakeGameOfRound(type, seed);
		game.Update(6.f);

		for (PlayerId id = 0; id < 4; ++id)
			game.Submit(id, FirstPainting(game, id));

		game.Update(3.f);
		game.Update(6.f);
		return game;
	}
}
