#include <gtest/gtest.h>

#include <vector>

#include <Game/LocalSession.h>
#include "TestGames.h"
using namespace lhs::game;
using namespace lhs::model;
using namespace lhs::test;


namespace
{
	LocalSession MakeSession(uint32_t seed = 1)
	{
		return LocalSession{ MakeCatalog(), { "You", "Andrew", "Benjamin", "Catherine" }, Random{ seed } };
	}

	void UpdateUntil(GameSession& session, Phase phase)
	{
		while (session.View().phase != phase)
			session.Update(.5f);
	}
}

TEST(LocalSessionTest, PlaysAsTheFirstPlayer)
{
	auto session = MakeSession();
	EXPECT_EQ(session.GetLocalPlayer(), 0u);
}

TEST(LocalSessionTest, TheComputersSubmitAsSoonAsTheGalleryOpens)
{
	auto session = MakeSession();
	session.StartGame();
	session.Update(6.f);

	ASSERT_EQ(session.View().phase, Phase::Submission);
	const auto& submissions = session.View().submissions;
	EXPECT_FALSE(submissions.at(0));
	EXPECT_TRUE(submissions.at(1) && submissions.at(2) && submissions.at(3));
}

TEST(LocalSessionTest, TheLocalPlayerSubmitsAndBids)
{
	auto session = MakeSession();
	session.StartGame();
	session.Update(6.f);

	const auto painting = session.View().players.at(0).GetPaintings().front();
	EXPECT_TRUE(session.Submit(painting));
	EXPECT_EQ(session.View().submissions.at(0), painting);

	UpdateUntil(session, Phase::Bidding);
	EXPECT_EQ(session.PlaceBid(0), BidError::BelowMinimum);
	EXPECT_EQ(session.PlaceBid(session.View().minimumBid), std::nullopt);
}

TEST(LocalSessionTest, TheComputersBidWhileTheAuctionIsOpen)
{
	std::vector<PlayerId> bidders;

	for (uint32_t seed = 1; seed <= 5; ++seed)
	{
		auto session = MakeSession(seed);
		session.StartGame();
		session.Update(6.f);
		session.Submit(session.View().players.at(0).GetPaintings().front());
		UpdateUntil(session, Phase::Closed);

		auto phase = Phase::Lobby;
		for (const auto& event : session.TakeEvents())
		{
			if (auto changed = std::get_if<PhaseChanged>(&event))
				phase = changed->phase;

			if (auto bid = std::get_if<BidPlaced>(&event))
			{
				EXPECT_EQ(phase, Phase::Bidding);
				bidders.push_back(bid->bidder);
			}
		}
	}

	EXPECT_FALSE(bidders.empty());
	for (auto bidder : bidders)
		EXPECT_NE(bidder, 0u);
}
