#include <gtest/gtest.h>

#include <map>
#include <numeric>
#include <vector>

#include <Game/LocalSession.h>
#include "TestGames.h"
using namespace lhs::game;
using namespace lhs::model;
using namespace lhs::test;


namespace
{
	uint32_t TotalGold(const GameView& view)
	{
		return std::accumulate(view.players.begin(), view.players.end(), 0u,
			[](uint32_t sum, const Player& player) { return sum + player.GetGold(); });
	}

	size_t TotalPaintings(const GameView& view)
	{
		return std::accumulate(view.players.begin(), view.players.end(), size_t{ 0 },
			[](size_t sum, const Player& player) { return sum + player.GetPaintings().size(); });
	}
}

// Play whole games at 60 frames per second against the computer, with the local player idle.
TEST(WholeGameTest, PlaysToTheRankingWithEveryRuleInPlace)
{
	for (uint32_t seed = 1; seed <= 5; ++seed)
	{
		SCOPED_TRACE(seed);
		LocalSession session{ MakeCatalog(), { "You", "Andrew", "Benjamin", "Catherine" }, Random{ seed } };

		std::map<Phase, int> phases;
		std::vector<AuctionClosed> results;
		uint32_t goldBeforeSale = 0;
		size_t paintingsBeforeSale = 0;

		session.StartGame();
		do
		{
			session.Update(1.f / 60);

			for (const auto& event : session.TakeEvents())
			{
				if (auto closed = std::get_if<AuctionClosed>(&event))
					results.push_back(*closed);

				if (auto changed = std::get_if<PhaseChanged>(&event))
				{
					++phases[changed->phase];

					if (changed->phase == Phase::RoundSettlement && session.View().round == 4)
					{
						goldBeforeSale = TotalGold(session.View());
						paintingsBeforeSale = TotalPaintings(session.View());
					}
				}
			}
		} while (session.View().phase != Phase::GameOver);

		// 4 rounds of 4 auctions
		EXPECT_EQ(phases[Phase::RoundIntro], 4);
		EXPECT_EQ(phases[Phase::Submission], 4);
		EXPECT_EQ(phases[Phase::Presenting], 16);
		EXPECT_EQ(phases[Phase::Bidding], 16);
		EXPECT_EQ(phases[Phase::Closed], 16);
		EXPECT_EQ(phases[Phase::RoundSettlement], 4);
		EXPECT_EQ(phases[Phase::GameOver], 1);
		ASSERT_EQ(results.size(), 16u);

		// The winners paid to no one, and the NFTs were sold for gold instead of kept.
		uint32_t paid = 0, nftSales = 0, nfts = 0;
		for (const auto& result : results)
		{
			paid += result.winningBid.amount;
			if (result.nftPrice)
			{
				nftSales += *result.nftPrice;
				++nfts;
			}
		}
		EXPECT_GT(paid, 0u);
		EXPECT_EQ(nfts, 4u);
		EXPECT_EQ(goldBeforeSale, 400u - paid + nftSales);
		EXPECT_EQ(paintingsBeforeSale, 40u - nfts);

		// Everyone sold their gallery, and the ranking is by gold.
		const auto& view = session.View();
		EXPECT_EQ(TotalPaintings(view), 0u);
		ASSERT_EQ(view.ranking.size(), 4u);
		for (size_t i = 1; i < view.ranking.size(); ++i)
			EXPECT_GE(view.players.at(view.ranking[i - 1]).GetGold(), view.players.at(view.ranking[i]).GetGold());
	}
}
