#include "SingleGameManager.h"
using namespace lhs::model;

#include <algorithm>
#include <optional>
#include <random>
#include <ranges>

#include <cocos2d.h>
#include <ccRandom.h>
USING_NS_CC;

#include <Manager/DbManager.h>


namespace lhs::manager
{
	uint32_t const SingleGameManager::MAX_USER_COUNT = 5;

	SingleGameManager::SingleGameManager()
		: nPlayers(0)
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

		for (const auto& painter : painters)
		{
			reputation.emplace(painter, 0);
			winningBids.push_back({ painter, 0 });
		}
	}

	bool SingleGameManager::IsBidUpdated() const
	{
		auto [_, round] = GetCurrentRound();

		return (round == u8"Fixed Price") ? !bids.empty() : false;
		//return (round == u8"Á¤Âû") ? !bids.empty() : false;
	}

	bool SingleGameManager::IsRoundEnd() const
	{
		return selections.empty();
	}

	bool SingleGameManager::HasAllUserSubmitted() const
	{
		return (selections.size() == nPlayers);
	}

	std::pair<int, std::u8string> SingleGameManager::GetCurrentRound() const
	{
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
				auto select = copy.at(RandomHelper::random_int<int>(0, copy.size() - 1));

				std::erase(copy, select);
				forGallery.push_back(select);
			}
			res.push_back(forGallery);
		}
		return res;
	}

	const Painting& SingleGameManager::GetSubmission() const
	{
		if (submission)
			return submission.value();

		// TODO: Handle exception
	}

	std::map<std::string, uint32_t> SingleGameManager::GetReputation() const
	{
		return reputation;
	}

	Painting SingleGameManager::GetSelectionForAuction()
	{
		CCASSERT(!selections.empty(), "selections must not empty.");
		auto idx = RandomHelper::random_int<int>(0, selections.size() - 1);
		auto selection = selections.at(idx);
		
		selections.erase(selections.begin() + idx);
		return selection;
	}

	std::vector<std::pair<int, int>> SingleGameManager::GetBids() const
	{
		auto view = bids | std::views::reverse;
		return { view.begin(), view.end() };
	}

	std::pair<int, int> SingleGameManager::GetLastBid() const
	{
		// If not all players have placed bids, a random player will be selected.
		CCASSERT(nPlayers > 1, "nPlayers must be greater than 1.");
		return bids.empty()
			? std::make_pair(cocos2d::RandomHelper::random_int<int>(0, nPlayers - 1), 0)
			: bids.back();
	}

	std::pair<int, int> SingleGameManager::GetWinningBid()
	{
		auto [_, round] = GetCurrentRound();

		if (round == u8"Closed Bidding")
			//if (round == u8"ºñ°ø°³ ÀÔÂû")
			return GetClosedWinningBid();

		auto winningBid = GetLastBid();
		bids.clear();
		return winningBid;
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

		// inhibit consecutive duplicated bids
		if (!bids.empty() && bid == bids.back())
			return;

		bids.push_back(bid);

		if (newBidEventListener)
			newBidEventListener(bid);
	}

	void SingleGameManager::MoveToNextRound()
	{
		if (!rounds.empty())
			rounds.pop_back();

		if (rounds.empty())
		{
			std::random_device rd;
			std::mt19937 generator(rd());
			rounds = { u8"Open Bidding", u8"Closed Bidding", u8"Fixed Price", u8"NFT Open Bidding" };
			//rounds = { u8"°ø°³ ÀÔÂû", u8"ºñ°ø°³ ÀÔÂû", u8"Á¤Âû", u8"NFT °ø°³ ÀÔÂû" };

			std::ranges::shuffle(rounds, generator);
		}
	}

	void SingleGameManager::AddNewBidEventListener(std::function<void(const std::pair<int, int>&)> listener) noexcept
	{
		newBidEventListener = listener;
	}

	void SingleGameManager::AddWinningBid(std::string_view painter, int winningBid)
	{
		// Find the painter of the sold painting.
		auto r = std::find_if(winningBids.begin(), winningBids.end(), [&](auto source)
			{
				return (source.first == painter.data());
			});
		if (r == winningBids.end())
			return;

		// Add the bidding price.
		r->second += winningBid;
	}

	void SingleGameManager::UpdateReputation()
	{
		// Sort the winning bids in ascending order.
		auto goldView = winningBids
			| std::views::transform([](auto& bid) { return bid.second; });

		std::set<int> s{ goldView.begin(), goldView.end() };
		
		// Update the painters' reputation.
		// Increase by 1, 3, 7, and 10 in ascending order of sales amount.
		std::vector<int> v = { 10, 7, 3, 0 };
		
		for (const auto& target : s)
		{
			auto view = winningBids
				| std::views::filter([&](const auto& src) { return (src.second == target); })
				| std::views::transform([](const auto& src) { return src.first; });

			for (const auto& painter : view)
				reputation.at(painter) +=  v.back();

			v.pop_back();
		}

		// Reset winningBids
		for (auto& [_, value] : winningBids)
			value = 0;
	}

	uint32_t SingleGameManager::GetRealTimeNftPrice()
	{
		uint32_t price = RandomHelper::random_int(0, 30);

		cocos2d::log("sold at %d", price);
		return price;
	}

	std::pair<int, int> SingleGameManager::GetClosedWinningBid()
	{
		// If not all players have placed bids, a random player will be selected.
		CCASSERT(nPlayers > 1, "nPlayers must be greater than 1.");
		auto winningBid = std::make_pair(RandomHelper::random_int<int>(0, nPlayers - 1), 0);

		while (!bids.empty())
		{
			auto bid = bids.back();

			std::erase_if(bids, [=](auto pair)
				{
					return (pair.first == bid.first);
				});

			if (bid.second > winningBid.second)
				winningBid = bid;
		}
		return winningBid;
	}
}
