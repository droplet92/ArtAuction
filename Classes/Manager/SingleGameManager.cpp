#include "SingleGameManager.h"
using namespace lhs::model;

#include <algorithm>
#include <functional>
#include <optional>
#include <random>
#include <ranges>

#include <cocos2d.h>

#include <Manager/DbManager.h>


namespace lhs::manager
{
	// The manager's interface still passes bids as (player id, amount) pairs.
	static std::pair<int, int> ToPair(const model::Bid& bid)
	{
		return { static_cast<int>(bid.bidder), static_cast<int>(bid.amount) };
	}

	uint32_t const SingleGameManager::MAX_USER_COUNT = 5;

	SingleGameManager::SingleGameManager()
		: nPlayers(0)
		, random(std::random_device{}())
		, prevBidsCount(0)
		, newBidEventListener({})
	{
		selections.reserve(MAX_USER_COUNT);
	}

	SingleGameManager& SingleGameManager::Instance()
	{
		static SingleGameManager manager{};
		return manager;
	}

	void SingleGameManager::Init()
	{
		paintings = DbManager::Instance().GetPaintings();
		painters = DbManager::Instance().GetPainters();

		// Reset the state left over from a previous game.
		rounds.clear();
		selections.clear();
		submission.reset();
		auction.reset();
		reputation = game::Reputation{ painters };
	}

	bool SingleGameManager::IsBidUpdated() const
	{
		return auction && auction->IsClosedEarly();
	}

	bool SingleGameManager::IsRoundEnd() const
	{
		return selections.empty();
	}

	bool SingleGameManager::HasAllUserSubmitted() const
	{
		return (selections.size() == nPlayers);
	}

	std::pair<int, RoundType> SingleGameManager::GetCurrentRound() const
	{
		CCASSERT(!rounds.empty(), "MoveToNextRound must be called before the first round.");
		return { 5 - rounds.size(), rounds.back() };
	}

	std::vector<std::string> SingleGameManager::GetPainters() const
	{
		return painters;
	}

	std::vector<std::vector<Painting>> SingleGameManager::GetPaintings(uint32_t nPlayers) const
	{
		std::vector<std::vector<Painting>> res;
		auto copy = paintings;

		for (uint32_t i = 0; i < nPlayers; i++)
		{
			std::vector<Painting> forGallery;

			for (int i = 0; i < 10; i++)
			{
				CCASSERT(!copy.empty(), "copy must not empty.");
				auto select = copy.at(game::RandomInt(random, 0, static_cast<int>(copy.size()) - 1));

				std::erase(copy, select);
				forGallery.push_back(select);
			}
			res.push_back(forGallery);
		}
		return res;
	}

	const Painting& SingleGameManager::GetSubmission() const
	{
		CCASSERT(submission, "a painting must be submitted.");
		return submission.value();
	}

	std::map<std::string, uint32_t> SingleGameManager::GetReputation() const
	{
		return reputation.Get();
	}

	Painting SingleGameManager::GetSelectionForAuction()
	{
		CCASSERT(!selections.empty(), "selections must not empty.");
		auto idx = game::RandomInt(random, 0, static_cast<int>(selections.size()) - 1);
		auto selection = selections.at(idx);
		
		selections.erase(selections.begin() + idx);

		// The painting goes on the easel, and its auction opens.
		auction.emplace(GetCurrentRound().second, nPlayers);
		return selection;
	}

	std::vector<std::pair<int, int>> SingleGameManager::GetBids() const
	{
		if (!auction)
			return {};

		auto view = auction->GetBids() | std::views::transform(ToPair);
		return { view.begin(), view.end() };
	}

	std::pair<int, int> SingleGameManager::GetLastBid() const
	{
		CCASSERT(auction, "an auction must be running.");
		return ToPair(auction->GetLastBid(random));
	}

	std::pair<int, int> SingleGameManager::GetWinningBid()
	{
		CCASSERT(auction, "an auction must be running.");
		return ToPair(auction->GetWinningBid(random));
	}

	void SingleGameManager::SetNumberOfPlayers(uint32_t nPlayers)
	{
		this->nPlayers = nPlayers;
	}

	void SingleGameManager::SubmitPainting(const Painting& painting)
	{
		submission = painting;
		selections.push_back(painting);
	}

	void SingleGameManager::Bid(const std::pair<int, int>& bid)
	{
		cocos2d::log("[Id %d] bid %d", bid.first, bid.second);

		if (!auction || !auction->Place({ static_cast<PlayerId>(bid.first), static_cast<Gold>(bid.second) }))
			return;

		if (newBidEventListener)
			newBidEventListener(bid);
	}

	void SingleGameManager::MoveToNextRound()
	{
		if (!rounds.empty())
			rounds.pop_back();

		if (rounds.empty())
		{
			rounds = { RoundType::OpenBidding, RoundType::ClosedBidding, RoundType::FixedPrice, RoundType::NftOpenBidding };

			std::ranges::shuffle(rounds, random);
		}
	}

	uint32_t SingleGameManager::GetMinimumBid() const
	{
		CCASSERT(auction, "an auction must be running.");
		return auction->GetMinimumBid();
	}

	void SingleGameManager::AddNewBidEventListener(std::function<void(const std::pair<int, int>&)> listener) noexcept
	{
		newBidEventListener = listener;
	}

	void SingleGameManager::AddWinningBid(std::string_view painter, int winningBid)
	{
		reputation.AddWinningBid(painter, winningBid);
	}

	void SingleGameManager::UpdateReputation()
	{
		reputation.Update();
	}

	uint32_t SingleGameManager::GetRealTimeNftPrice()
	{
		uint32_t price = game::RollNftPrice(random);

		cocos2d::log("sold at %d", price);
		return price;
	}
}
