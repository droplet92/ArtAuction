#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include <Game/Game.h>
using namespace lhs::game;
using namespace lhs::model;


namespace
{
	// 5 painters with 10 paintings each, like the game data
	Catalog MakeCatalog()
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

	Game MakeGame(uint32_t seed = 1)
	{
		return Game{ MakeCatalog(), { "You", "Andrew", "Benjamin", "Catherine" }, Random{ seed } };
	}
}

TEST(GameTest, WaitsInTheLobbyUntilStarted)
{
	auto game = MakeGame();
	game.Update(100.f);

	EXPECT_EQ(game.View().phase, Phase::Lobby);
	EXPECT_TRUE(game.View().players.empty());
}

TEST(GameTest, StartDealsTenDifferentPaintingsToEachPlayer)
{
	auto game = MakeGame();
	game.Start();

	const auto& players = game.View().players;
	ASSERT_EQ(players.size(), 4u);

	std::set<std::string> dealt;
	for (PlayerId id = 0; id < players.size(); ++id)
	{
		EXPECT_EQ(players[id].GetId(), id);
		EXPECT_EQ(players[id].GetGold(), 100u);
		ASSERT_EQ(players[id].GetPaintings().size(), 10u);

		for (const auto& painting : players[id].GetPaintings())
			dealt.insert(painting.title);
	}
	EXPECT_EQ(dealt.size(), 40u);
}

TEST(GameTest, StartsWithNoReputation)
{
	auto game = MakeGame();
	game.Start();

	ASSERT_EQ(game.View().reputation.size(), 5u);
	for (const auto& [_, points] : game.View().reputation)
		EXPECT_EQ(points, 0u);
}

namespace
{
	// Take the game's events and return those of the given type.
	template <typename Event>
	std::vector<Event> TakeEventsOf(Game& game)
	{
		std::vector<Event> taken;

		for (const auto& event : game.TakeEvents())
		{
			if (auto found = std::get_if<Event>(&event))
				taken.push_back(*found);
		}
		return taken;
	}
}

TEST(GameTest, ExplainsTheFirstRoundForSixSeconds)
{
	auto game = MakeGame();

	game.Start();
	EXPECT_EQ(game.View().round, 1);
	EXPECT_EQ(game.View().phase, Phase::RoundIntro);

	game.Update(5.9f);
	EXPECT_EQ(game.View().phase, Phase::RoundIntro);

	game.Update(.1f);
	EXPECT_EQ(game.View().phase, Phase::Submission);

	std::vector<Phase> phases;
	for (const auto& changed : TakeEventsOf<PhaseChanged>(game))
		phases.push_back(changed.phase);
	EXPECT_EQ(phases, (std::vector<Phase>{ Phase::RoundIntro, Phase::Submission }));
	EXPECT_TRUE(game.TakeEvents().empty());
}

TEST(GameTest, TheSameSeedDealsTheSameGame)
{
	auto first = MakeGame(7);
	auto second = MakeGame(7);
	first.Start();
	second.Start();

	EXPECT_EQ(first.View().roundType, second.View().roundType);
	EXPECT_EQ(first.View().players[0].GetPaintings(), second.View().players[0].GetPaintings());
}
