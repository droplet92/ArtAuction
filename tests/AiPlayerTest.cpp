#include <gtest/gtest.h>

#include <set>
#include <vector>

#include <Game/AiPlayer.h>
#include "TestGames.h"
using namespace lhs::game;
using namespace lhs::model;
using namespace lhs::test;


namespace
{
	struct Record
	{
		// the seconds into the bidding of each bid by the computer player
		std::vector<float> times;

		// all the bids of the auction, from the latest
		std::vector<Bid> bids;
	};

	// Let the computer player bid alone until the auction closes.
	Record RunBidding(Game& game, AiPlayer& ai, uint32_t seed)
	{
		Random random{ seed };
		Record record;
		float elapsed = 0.f;

		ai.BeginBidding(game.View().roundType);
		while (game.View().phase == Phase::Bidding)
		{
			elapsed += .5f;
			ai.Update(.5f, game, random);
			game.Update(.5f);

			for (const auto& event : game.TakeEvents())
			{
				if (auto bid = std::get_if<BidPlaced>(&event); bid && bid->bidder == ai.GetId())
					record.times.push_back(elapsed);
				if (auto closed = std::get_if<AuctionClosed>(&event))
					record.bids = closed->bids;
			}
		}
		return record;
	}

	// The seconds at which the computer player bid in any of many auctions.
	std::set<float> BidTimes(RoundType type, PlayerId id)
	{
		std::set<float> times;

		for (uint32_t seed = 1; seed <= 40; ++seed)
		{
			auto game = MakeGameInBidding(type);
			AiPlayer ai{ id };

			for (auto time : RunBidding(game, ai, seed).times)
				times.insert(time);
		}
		return times;
	}
}

TEST(AiPlayerTest, SubmitsThePaintingAtItsOwnPlaceInTheGallery)
{
	auto game = MakeGame();
	game.Start();

	for (PlayerId id = 1; id <= 3; ++id)
	{
		const auto& player = game.View().players.at(id);
		EXPECT_EQ(AiPlayer{ id }.ChooseSubmission(player), player.GetPaintings().at(id));
	}
}

TEST(AiPlayerTest, BidsOnTheOpenBiddingSchedule)
{
	EXPECT_EQ(BidTimes(RoundType::OpenBidding, 1), (std::set<float>{ 3, 6, 9, 12, 15, 18, 21, 24, 27, 30 }));
	EXPECT_EQ(BidTimes(RoundType::OpenBidding, 2), (std::set<float>{ 1, 6, 11, 16 }));
	EXPECT_EQ(BidTimes(RoundType::OpenBidding, 3), (std::set<float>{ 7, 14, 21 }));
}

TEST(AiPlayerTest, BidsOnTheNftSchedule)
{
	EXPECT_EQ(BidTimes(RoundType::NftOpenBidding, 1), (std::set<float>{ 3, 6, 9, 12, 15, 18, 21, 24, 27, 30 }));
	EXPECT_EQ(BidTimes(RoundType::NftOpenBidding, 2), (std::set<float>{ 1, 6, 11, 16 }));
	EXPECT_EQ(BidTimes(RoundType::NftOpenBidding, 3), (std::set<float>{ 7, 14, 21 }));
}

TEST(AiPlayerTest, BidsOnceInAClosedBiddingRound)
{
	EXPECT_EQ(BidTimes(RoundType::ClosedBidding, 1), (std::set<float>{ 3 }));
	EXPECT_EQ(BidTimes(RoundType::ClosedBidding, 2), (std::set<float>{ 10 }));
	EXPECT_EQ(BidTimes(RoundType::ClosedBidding, 3), (std::set<float>{ 14 }));
}

TEST(AiPlayerTest, BidsOnTheFixedPriceSchedule)
{
	EXPECT_EQ(BidTimes(RoundType::FixedPrice, 1), (std::set<float>{ 1, 6, 11, 16 }));
	EXPECT_EQ(BidTimes(RoundType::FixedPrice, 2), (std::set<float>{ 10 }));
	EXPECT_EQ(BidTimes(RoundType::FixedPrice, 3), (std::set<float>{ 15 }));
}

TEST(AiPlayerTest, OffersTheMinimumBidInVisibleRounds)
{
	for (uint32_t seed = 1; seed <= 10; ++seed)
	{
		auto game = MakeGameInBidding(RoundType::OpenBidding);
		AiPlayer ai{ 1 };
		const auto bids = RunBidding(game, ai, seed).bids;

		// Each bid offers one more than the one before it, starting from 1.
		for (size_t i = 0; i < bids.size(); ++i)
			EXPECT_EQ(bids[i].amount, bids.size() - i);
	}
}

TEST(AiPlayerTest, OffersUpToTwentyInAClosedRound)
{
	std::set<Gold> amounts;

	for (uint32_t seed = 1; seed <= 40; ++seed)
	{
		auto game = MakeGameInBidding(RoundType::ClosedBidding);
		AiPlayer ai{ 1 };

		for (const auto& bid : RunBidding(game, ai, seed).bids)
			amounts.insert(bid.amount);
	}
	ASSERT_FALSE(amounts.empty());
	EXPECT_GE(*amounts.begin(), 1u);
	EXPECT_LE(*amounts.rbegin(), 20u);
	EXPECT_GT(amounts.size(), 1u);
}

TEST(AiPlayerTest, StopsBiddingWithoutMoreGoldThanTheMinimum)
{
	for (uint32_t seed = 1; seed <= 10; ++seed)
	{
		auto game = MakeGameInBidding(RoundType::OpenBidding);
		game.PlaceBid({ 0, 100 });	// The minimum bid becomes 101.

		AiPlayer ai{ 1 };
		EXPECT_TRUE(RunBidding(game, ai, seed).times.empty());
	}
}
