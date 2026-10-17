#include <gtest/gtest.h>

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <Game/Game.h>
#include "TestGames.h"
using namespace lhs::game;
using namespace lhs::model;
using namespace lhs::test;


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

	game.Update(5.5f);
	EXPECT_EQ(game.View().phase, Phase::RoundIntro);

	game.Update(.5f);
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

namespace
{
	// Start a game and skip the round explanation.
	Game MakeGameInSubmission(uint32_t seed = 1)
	{
		auto game = MakeGame(seed);
		game.Start();
		game.Update(6.f);
		return game;
	}}

TEST(GameTest, SubmittingMovesThePaintingOutOfTheGallery)
{
	auto game = MakeGameInSubmission();
	const auto painting = FirstPainting(game, 0);

	EXPECT_TRUE(game.Submit(0, painting));
	EXPECT_EQ(game.View().submissions.at(0), painting);
	EXPECT_EQ(game.View().players.at(0).GetPaintings().size(), 9u);
}

TEST(GameTest, RejectsASecondSubmissionAndPaintingsFromOtherGalleries)
{
	auto game = MakeGameInSubmission();

	EXPECT_FALSE(game.Submit(0, FirstPainting(game, 1)));
	EXPECT_TRUE(game.Submit(0, FirstPainting(game, 0)));
	EXPECT_FALSE(game.Submit(0, FirstPainting(game, 0)));
}

TEST(GameTest, RejectsSubmissionsOutsideTheSubmissionPhase)
{
	auto game = MakeGame();
	game.Start();

	EXPECT_FALSE(game.Submit(0, FirstPainting(game, 0)));
}

TEST(GameTest, ShowsTheSubmissionsForThreeSecondsAfterTheLastOne)
{
	auto game = MakeGameInSubmission();

	for (PlayerId id = 0; id < 4; ++id)
		game.Submit(id, FirstPainting(game, id));

	EXPECT_FLOAT_EQ(game.View().timeLeft, 3.f);
}

TEST(GameTest, SubmitsARandomPaintingForThoseWhoRunOutOfTime)
{
	auto game = MakeGameInSubmission();
	game.Submit(1, FirstPainting(game, 1));

	game.Update(30.5f);
	EXPECT_FALSE(game.View().submissions.at(0));

	game.Update(.5f);
	for (PlayerId id = 0; id < 4; ++id)
	{
		EXPECT_TRUE(game.View().submissions.at(id));
		EXPECT_EQ(game.View().players.at(id).GetPaintings().size(), 9u);
	}
	EXPECT_FLOAT_EQ(game.View().timeLeft, 3.f);
}

TEST(GameTest, PresentsASubmittedPaintingForSixSecondsBeforeBidding)
{
	auto game = MakeGameOfRound(RoundType::OpenBidding);
	game.Update(6.f);

	std::vector<Painting> submitted;
	for (PlayerId id = 0; id < 4; ++id)
	{
		submitted.push_back(FirstPainting(game, id));
		game.Submit(id, submitted.back());
	}
	game.Update(3.f);

	EXPECT_EQ(game.View().phase, Phase::Presenting);
	ASSERT_TRUE(game.View().painting);
	EXPECT_NE(std::ranges::find(submitted, *game.View().painting), submitted.end());
	EXPECT_EQ(game.PlaceBid({ 0, 5 }), BidError::NotBidding);

	game.Update(6.f);
	EXPECT_EQ(game.View().phase, Phase::Bidding);
	EXPECT_FLOAT_EQ(game.View().timeLeft, 32.f);
}

TEST(GameTest, ChecksABidAgainstTheMinimumAndTheBiddersGold)
{
	auto game = MakeGameInBidding(RoundType::OpenBidding);

	EXPECT_EQ(game.PlaceBid({ 0, 0 }), BidError::BelowMinimum);
	EXPECT_EQ(game.PlaceBid({ 0, 101 }), BidError::NotEnoughGold);
	EXPECT_EQ(game.PlaceBid({ 0, 100 }), std::nullopt);
	EXPECT_EQ(game.View().minimumBid, 101u);
}

TEST(GameTest, HidesTheAmountOfClosedBids)
{
	auto open = MakeGameInBidding(RoundType::OpenBidding);
	auto closed = MakeGameInBidding(RoundType::ClosedBidding);

	open.PlaceBid({ 2, 7 });
	closed.PlaceBid({ 2, 7 });

	const auto openBids = TakeEventsOf<BidPlaced>(open);
	const auto closedBids = TakeEventsOf<BidPlaced>(closed);
	ASSERT_EQ(openBids.size(), 1u);
	EXPECT_EQ(openBids[0].bidder, 2u);
	EXPECT_EQ(openBids[0].amount, 7u);
	ASSERT_EQ(closedBids.size(), 1u);
	EXPECT_EQ(closedBids[0].amount, std::nullopt);
}

TEST(GameTest, ClosesAfterThirtyTwoSecondsOfBidding)
{
	auto game = MakeGameInBidding(RoundType::OpenBidding);

	game.Update(31.5f);
	EXPECT_EQ(game.View().phase, Phase::Bidding);

	game.Update(.5f);
	EXPECT_EQ(game.View().phase, Phase::Closed);
	EXPECT_EQ(game.PlaceBid({ 0, 5 }), BidError::NotBidding);
}

TEST(GameTest, ClosesAFixedPriceAuctionAtTheFirstBid)
{
	auto game = MakeGameInBidding(RoundType::FixedPrice);

	EXPECT_EQ(game.View().minimumBid, 10u);
	game.PlaceBid({ 3, 10 });
	EXPECT_EQ(game.View().phase, Phase::Closed);
	EXPECT_FLOAT_EQ(game.View().timeLeft, 2.f);
}

TEST(GameTest, TakesLongerToRevealClosedBids)
{
	auto game = MakeGameInBidding(RoundType::ClosedBidding);
	game.Update(32.f);

	EXPECT_EQ(game.View().phase, Phase::Closed);
	EXPECT_FLOAT_EQ(game.View().timeLeft, 9.f);
}

TEST(GameTest, AuctionsEverySubmissionOnceBeforeTheRoundSettles)
{
	auto game = MakeGameInBidding(RoundType::OpenBidding);

	std::vector<std::string> auctioned{ game.View().painting->title };
	for (int i = 0; i < 3; ++i)
	{
		game.Update(32.f);	// bidding
		game.Update(2.f);	// reveal
		EXPECT_EQ(game.View().phase, Phase::Presenting);
		auctioned.push_back(game.View().painting->title);
		game.Update(6.f);
	}
	game.Update(32.f);
	game.Update(2.f);

	EXPECT_EQ(game.View().phase, Phase::RoundSettlement);
	std::ranges::sort(auctioned);
	EXPECT_EQ(std::ranges::adjacent_find(auctioned), auctioned.end());
}

TEST(GameTest, TheWinnerPaysAndTakesThePainting)
{
	auto game = MakeGameInBidding(RoundType::OpenBidding);
	const auto painting = *game.View().painting;

	game.PlaceBid({ 1, 5 });
	game.PlaceBid({ 2, 8 });
	game.Update(32.f);

	const auto results = TakeEventsOf<AuctionClosed>(game);
	ASSERT_EQ(results.size(), 1u);
	const auto& result = results.front();
	EXPECT_EQ(result.winningBid, (Bid{ 2, 8 }));
	EXPECT_EQ(result.painting, painting);
	EXPECT_EQ(result.nftPrice, std::nullopt);
	EXPECT_EQ(result.bids, (std::vector<Bid>{ { 2, 8 }, { 1, 5 } }));

	const auto& winner = game.View().players.at(2);
	EXPECT_EQ(winner.GetGold(), 92u);
	const auto gallery = winner.GetPaintings();
	EXPECT_NE(std::ranges::find(gallery, painting), gallery.end());
	EXPECT_EQ(game.View().players.at(1).GetGold(), 100u);
}

TEST(GameTest, TheWinnerOfAnNftSellsItAtOnce)
{
	auto game = MakeGameInBidding(RoundType::NftOpenBidding);
	const auto galleryBefore = game.View().players.at(3).GetPaintings().size();

	game.PlaceBid({ 3, 20 });
	game.Update(32.f);

	const auto results = TakeEventsOf<AuctionClosed>(game);
	ASSERT_EQ(results.size(), 1u);
	const auto& result = results.front();
	ASSERT_TRUE(result.nftPrice);
	EXPECT_LE(*result.nftPrice, 30u);
	EXPECT_EQ(game.View().players.at(3).GetGold(), 100u - 20u + *result.nftPrice);
	EXPECT_EQ(game.View().players.at(3).GetPaintings().size(), galleryBefore);
}

TEST(GameTest, AnAuctionWithoutBidsGoesToSomeoneForNothing)
{
	auto game = MakeGameInBidding(RoundType::OpenBidding);
	game.Update(32.f);

	const auto results = TakeEventsOf<AuctionClosed>(game);
	ASSERT_EQ(results.size(), 1u);
	const auto& result = results.front();
	EXPECT_EQ(result.winningBid.amount, 0u);
	EXPECT_LT(result.winningBid.bidder, 4u);
	EXPECT_EQ(game.View().players.at(result.winningBid.bidder).GetGold(), 100u);
}

namespace
{
	// Play the auctions of the round without anyone bidding, up to the round settlement.
	void PlayAuctionsWithoutBids(Game& game)
	{
		while (game.View().phase != Phase::RoundSettlement)
			game.Update(.5f);
	}

	// Play a round from its explanation to the next one, with everyone submitting their first painting.
	void PlayRound(Game& game)
	{
		game.Update(6.f);
		for (PlayerId id = 0; id < 4; ++id)
			game.Submit(id, FirstPainting(game, id));

		PlayAuctionsWithoutBids(game);
		game.Update(11.f);
	}
}

TEST(GameTest, RanksThePaintersAfterTheLastAuctionOfTheRound)
{
	auto game = MakeGameInBidding(RoundType::OpenBidding);
	const auto painter = game.View().painting->painter;

	game.PlaceBid({ 0, 5 });
	PlayAuctionsWithoutBids(game);

	EXPECT_EQ(game.View().reputation.at(painter), 10u);
	EXPECT_FALSE(game.View().painting);
	EXPECT_FLOAT_EQ(game.View().timeLeft, 11.f);
}

TEST(GameTest, PlaysEveryRoundTypeOnceAndRanksThePlayers)
{
	auto game = MakeGame();
	game.Start();

	std::set<RoundType> played;
	for (int round = 1; round <= 4; ++round)
	{
		EXPECT_EQ(game.View().phase, Phase::RoundIntro);
		EXPECT_EQ(game.View().round, round);
		played.insert(game.View().roundType);
		PlayRound(game);
	}

	EXPECT_EQ(played.size(), 4u);
	ASSERT_EQ(game.View().phase, Phase::GameOver);

	const auto& view = game.View();
	ASSERT_EQ(view.ranking.size(), 4u);
	for (size_t i = 1; i < view.ranking.size(); ++i)
		EXPECT_GE(view.players.at(view.ranking[i - 1]).GetGold(), view.players.at(view.ranking[i]).GetGold());
	for (const auto& player : view.players)
		EXPECT_TRUE(player.GetPaintings().empty());
}

TEST(GameTest, SellsTheGalleriesAtTheReputationWhenTheGameEnds)
{
	auto game = MakeGame();
	game.Start();
	for (int round = 1; round <= 3; ++round)
		PlayRound(game);

	// Play the last round up to its settlement, where the final reputation is known.
	// One painting is sold so that its painter earns points.
	game.Update(6.f);
	for (PlayerId id = 0; id < 4; ++id)
		game.Submit(id, FirstPainting(game, id));
	while (game.View().phase != Phase::Bidding)
		game.Update(.5f);

	const auto painter = game.View().painting->painter;
	game.PlaceBid({ 0, game.View().minimumBid });
	PlayAuctionsWithoutBids(game);
	EXPECT_GE(game.View().reputation.at(painter), 10u);

	std::vector<uint32_t> expected;
	for (const auto& player : game.View().players)
	{
		auto gold = player.GetGold();
		for (const auto& painting : player.GetPaintings())
			gold += game.View().reputation.at(painting.painter);
		expected.push_back(gold);
	}

	game.Update(11.f);
	ASSERT_EQ(game.View().phase, Phase::GameOver);
	for (PlayerId id = 0; id < 4; ++id)
		EXPECT_EQ(game.View().players.at(id).GetGold(), expected.at(id));
}

TEST(GameTest, AFinishedGameStartsOver)
{
	auto game = MakeGame();
	game.Start();
	for (int round = 1; round <= 4; ++round)
		PlayRound(game);
	ASSERT_EQ(game.View().phase, Phase::GameOver);

	game.Start();
	EXPECT_EQ(game.View().phase, Phase::RoundIntro);
	EXPECT_EQ(game.View().round, 1);
	EXPECT_TRUE(game.View().ranking.empty());
	for (const auto& player : game.View().players)
	{
		EXPECT_EQ(player.GetGold(), 100u);
		EXPECT_EQ(player.GetPaintings().size(), 10u);
	}
	for (const auto& [_, points] : game.View().reputation)
		EXPECT_EQ(points, 0u);
}
