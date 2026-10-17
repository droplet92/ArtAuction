#include <gtest/gtest.h>

#include <Game/Reputation.h>
using namespace lhs::game;


namespace
{
	const std::vector<std::string> painters{ "Goya", "Hokusai", "Kandinsky", "van Gogh", "AI" };
}

TEST(ReputationTest, StartsEveryPainterAtZero)
{
	const Reputation reputation{ painters };

	ASSERT_EQ(reputation.Get().size(), painters.size());
	for (const auto& [painter, points] : reputation.Get())
		EXPECT_EQ(points, 0u) << painter;
}

TEST(ReputationTest, RewardsTheThreeBestSellers)
{
	Reputation reputation{ painters };
	reputation.AddWinningBid("Goya", 20);
	reputation.AddWinningBid("Hokusai", 15);
	reputation.AddWinningBid("Kandinsky", 9);
	reputation.AddWinningBid("van Gogh", 2);

	reputation.Update();

	const std::map<std::string, uint32_t> expected{ { "Goya", 10 }, { "Hokusai", 7 }, { "Kandinsky", 3 }, { "van Gogh", 0 }, { "AI", 0 } };
	EXPECT_EQ(reputation.Get(), expected);
}

TEST(ReputationTest, AddsUpSalesOfTheSamePainter)
{
	Reputation reputation{ painters };
	reputation.AddWinningBid("Hokusai", 6);
	reputation.AddWinningBid("Hokusai", 6);
	reputation.AddWinningBid("Goya", 10);

	reputation.Update();

	EXPECT_EQ(reputation.Get().at("Hokusai"), 10u);
	EXPECT_EQ(reputation.Get().at("Goya"), 7u);
}

TEST(ReputationTest, GivesTiedPaintersTheSamePoints)
{
	Reputation reputation{ painters };
	reputation.AddWinningBid("Goya", 10);
	reputation.AddWinningBid("Hokusai", 10);
	reputation.AddWinningBid("Kandinsky", 4);

	reputation.Update();

	EXPECT_EQ(reputation.Get().at("Goya"), 10u);
	EXPECT_EQ(reputation.Get().at("Hokusai"), 10u);
	EXPECT_EQ(reputation.Get().at("Kandinsky"), 7u);
}

TEST(ReputationTest, GivesNothingForNoSales)
{
	Reputation reputation{ painters };
	reputation.AddWinningBid("Goya", 0);

	reputation.Update();

	EXPECT_EQ(reputation.Get().at("Goya"), 0u);
}

TEST(ReputationTest, AccumulatesOverRoundsWithFreshSales)
{
	Reputation reputation{ painters };
	reputation.AddWinningBid("Goya", 5);
	reputation.Update();

	reputation.AddWinningBid("Hokusai", 5);
	reputation.Update();

	EXPECT_EQ(reputation.Get().at("Goya"), 10u);
	EXPECT_EQ(reputation.Get().at("Hokusai"), 10u);
}

TEST(ReputationTest, IgnoresUnknownPainters)
{
	Reputation reputation{ painters };
	reputation.AddWinningBid("Picasso", 50);

	reputation.Update();

	EXPECT_EQ(reputation.Get().count("Picasso"), 0u);
}
