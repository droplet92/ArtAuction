#include <gtest/gtest.h>

#include <Game/Auction.h>
using namespace lhs::game;
using namespace lhs::model;


namespace
{
	constexpr uint32_t nPlayers = 4;
}

TEST(AuctionTest, FixedPriceStartsAtTenAndOtherRoundsAtOne)
{
	EXPECT_EQ(Auction(RoundType::FixedPrice, nPlayers).GetMinimumBid(), 10u);
	EXPECT_EQ(Auction(RoundType::OpenBidding, nPlayers).GetMinimumBid(), 1u);
	EXPECT_EQ(Auction(RoundType::ClosedBidding, nPlayers).GetMinimumBid(), 1u);
	EXPECT_EQ(Auction(RoundType::NftOpenBidding, nPlayers).GetMinimumBid(), 1u);
}

TEST(AuctionTest, VisibleBidsRaiseTheMinimumBid)
{
	Auction open{ RoundType::OpenBidding, nPlayers };
	open.Place({ 1, 5 });
	EXPECT_EQ(open.GetMinimumBid(), 6u);

	Auction closed{ RoundType::ClosedBidding, nPlayers };
	closed.Place({ 1, 5 });
	EXPECT_EQ(closed.GetMinimumBid(), 1u);
}

TEST(AuctionTest, IgnoresABidThatRepeatsThePreviousOne)
{
	Auction auction{ RoundType::OpenBidding, nPlayers };

	EXPECT_TRUE(auction.Place({ 1, 5 }));
	EXPECT_FALSE(auction.Place({ 1, 5 }));
	EXPECT_TRUE(auction.Place({ 2, 5 }));
	EXPECT_EQ(auction.GetBids().size(), 2u);
}

TEST(AuctionTest, ListsBidsFromTheLatest)
{
	Auction auction{ RoundType::ClosedBidding, nPlayers };
	auction.Place({ 1, 3 });
	auction.Place({ 2, 4 });

	const std::vector<Bid> expected{ { 2, 4 }, { 1, 3 } };
	EXPECT_EQ(auction.GetBids(), expected);
}

TEST(AuctionTest, OnlyFixedPriceClosesAtTheFirstBid)
{
	Auction fixed{ RoundType::FixedPrice, nPlayers };
	EXPECT_FALSE(fixed.IsClosedEarly());
	fixed.Place({ 2, 10 });
	EXPECT_TRUE(fixed.IsClosedEarly());

	Auction open{ RoundType::OpenBidding, nPlayers };
	open.Place({ 2, 10 });
	EXPECT_FALSE(open.IsClosedEarly());
}

TEST(AuctionTest, OpenBiddingGoesToTheLastBid)
{
	Random random{ 1 };
	Auction auction{ RoundType::OpenBidding, nPlayers };
	auction.Place({ 1, 3 });
	auction.Place({ 2, 4 });
	auction.Place({ 1, 5 });

	EXPECT_EQ(auction.GetWinningBid(random), (Bid{ 1, 5 }));
}

TEST(AuctionTest, ClosedBiddingCountsEachPlayersLatestBid)
{
	Random random{ 1 };
	Auction auction{ RoundType::ClosedBidding, nPlayers };
	auction.Place({ 1, 9 });
	auction.Place({ 2, 6 });
	auction.Place({ 1, 4 });	// player 1 lowers their offer

	EXPECT_EQ(auction.GetWinningBid(random), (Bid{ 2, 6 }));
}

TEST(AuctionTest, ClosedBiddingTieGoesToTheLaterBidder)
{
	Random random{ 1 };
	Auction auction{ RoundType::ClosedBidding, nPlayers };
	auction.Place({ 1, 7 });
	auction.Place({ 3, 7 });

	EXPECT_EQ(auction.GetWinningBid(random), (Bid{ 3, 7 }));
}

TEST(AuctionTest, UnbidPaintingGoesToARandomPlayerForNothing)
{
	for (auto type : { RoundType::OpenBidding, RoundType::ClosedBidding, RoundType::FixedPrice, RoundType::NftOpenBidding })
	{
		Random random{ 7 };
		const auto winner = Auction{ type, nPlayers }.GetWinningBid(random);

		EXPECT_LT(winner.bidder, nPlayers);
		EXPECT_EQ(winner.amount, 0u);
	}
}

TEST(AuctionTest, NftPriceStaysWithinZeroToThirty)
{
	Random random{ 3 };

	for (int i = 0; i < 1000; i++)
		EXPECT_LE(RollNftPrice(random), 30u);
}

TEST(AuctionTest, DecidingTheWinnerClearsTheBids)
{
	for (auto type : { RoundType::OpenBidding, RoundType::ClosedBidding, RoundType::FixedPrice })
	{
		Random random{ 1 };
		Auction auction{ type, nPlayers };
		auction.Place({ 1, 10 });

		auction.GetWinningBid(random);

		EXPECT_TRUE(auction.GetBids().empty());
		EXPECT_FALSE(auction.IsClosedEarly());
	}
}
