#include "AiPlayer.h"

#include <algorithm>
#include <cassert>

using namespace lhs::model;


namespace lhs::game
{
	namespace
	{
		// A repeating timer: the first try, the interval between tries, and the number of tries.
		struct Timer
		{
			float first;
			float interval;
			int count;
		};

		Timer GetTimer(RoundType type, PlayerId id)
		{
			switch (type)
			{
			case RoundType::OpenBidding: [[fallthrough]];
			case RoundType::NftOpenBidding:
				if (id == 1) return { 3.f, 3.f, 10 };
				if (id == 2) return { 1.f, 5.f, 4 };
				return { 7.f, 7.f, 3 };
			case RoundType::ClosedBidding:
				if (id == 1) return { 3.f, 0.f, 1 };
				if (id == 2) return { 10.f, 0.f, 1 };
				return { 14.f, 0.f, 1 };
			case RoundType::FixedPrice:
				if (id == 1) return { 1.f, 5.f, 4 };
				if (id == 2) return { 10.f, 0.f, 1 };
				return { 15.f, 0.f, 1 };
			}
			return { 0.f, 0.f, 0 };
		}
	}

	AiPlayer::AiPlayer(PlayerId id)
		: id(id)
		, roundType(RoundType::OpenBidding)
		, next(0)
		, elapsed(0.f)
	{
		assert(1 <= id && id <= 3 && "the computer players are the players 1 to 3.");
	}

	PlayerId AiPlayer::GetId() const noexcept
	{
		return id;
	}

	Painting AiPlayer::ChooseSubmission(const Player& self) const
	{
		// TODO: Each computer player always submits the painting at its own id in the gallery.
		// Choosing better is left for after the refactoring.
		return self.GetPaintings().at(id);
	}

	void AiPlayer::BeginBidding(RoundType type)
	{
		roundType = type;
		elapsed = 0.f;
		next = 0;

		const auto timer = GetTimer(type, id);
		schedule.clear();
		for (int i = 0; i < timer.count; ++i)
			schedule.push_back(timer.first + timer.interval * i);
	}

	void AiPlayer::Update(float dt, Game& game, Random& random)
	{
		elapsed += dt;

		while (next < schedule.size() && elapsed >= schedule[next])
		{
			++next;

			// A fixed-price auction may have closed at the previous bid.
			if (game.View().phase == Phase::Bidding)
				TryBid(game, random);
		}
	}

	void AiPlayer::TryBid(Game& game, Random& random) const
	{
		const auto gold = game.View().players.at(id).GetGold();
		const auto minBid = game.View().minimumBid;

		if (roundType == RoundType::ClosedBidding)
		{
			// a random amount up to 20
			auto bid = std::min(20u, gold);
			if (bid < minBid) return;
			if (!RandomInt(random, 0, 1)) return;
			game.PlaceBid({ id, static_cast<Gold>(RandomInt(random, static_cast<int>(minBid), static_cast<int>(bid))) });
		}
		else
		{
			// the minimum bid, while having more gold than that
			if (gold <= minBid) return;
			if (!RandomInt(random, 0, 1)) return;
			game.PlaceBid({ id, minBid });
		}
	}
}
