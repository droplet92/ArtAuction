#include "SingleGameManager.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>

#include <cocos2d.h>
#include <ccRandom.h>


namespace lhs::Manager
{
	size_t const SingleGameManager::MAX_USER_COUNT = 5;

	SingleGameManager::SingleGameManager()
		: nPlayers(0)
		, rounds()
		, submission(nullptr)
		, prevBidsCount(0)
	{
		selections.reserve(MAX_USER_COUNT);
	}

	SingleGameManager& SingleGameManager::Instance()
	{
		static SingleGameManager* manager = nullptr;

		if (!manager)
			manager = new SingleGameManager;
		return *manager;
	}

	// sqlite 연동
	void SingleGameManager::Init()
	{
		try
		{
			std::filesystem::directory_iterator root{ "../Resources/paintings" };

			for (const auto& directory : root)
			{
				std::filesystem::directory_iterator iter{ directory.path() };
				std::cout << directory.path().string() << std::endl;

				for (const auto& entry : iter)
				{
					//std::cout << entry.path().string() << std::endl;

					auto painting = new Model::Painting;

					std::stringstream ss{ entry.path().stem().string() };

					std::getline(ss, painting->painter, '-');
					std::getline(ss, painting->title, '-');
					painting->painter.pop_back();

					painting->path = entry.path();
					painting->title = painting->title.substr(1);

					paintings.push_back(painting);
					painters.insert(painting->painter);
				}
			}

			for (const auto& painter : painters)
				reputation.emplace(painter, 0);
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	void SingleGameManager::SetNumberOfPlayers(size_t nPlayers)
	{
		this->nPlayers = nPlayers;
	}

	void SingleGameManager::MoveToNextRound()
	{
		if (!rounds.empty())
			rounds.pop_back();

		if (rounds.empty())
		{
			std::random_device rd;
			std::mt19937 generator(rd());
			rounds = { u8"공개 입찰", u8"비공개 입찰", u8"정찰", u8"NFT 공개 입찰" };

			std::ranges::shuffle(rounds, generator);
		}
	}

	std::pair<int, std::u8string> SingleGameManager::GetCurrentRound() const
	{
		return { 5 - rounds.size(), rounds.back() };
	}

	std::vector<std::vector<Model::Painting*>> SingleGameManager::GetPaintings(size_t nPlayers) const
	{
		std::vector<std::vector<Model::Painting*>> res;
		auto copy = paintings;

		for (size_t i = 0; i < nPlayers; i++)
		{
			std::vector<Model::Painting*> forGallery;

			for (int i = 0; i < 10; i++)
			{
				auto select = copy.at(cocos2d::RandomHelper::random_int<int>(0, copy.size() - 1));

				std::erase(copy, select);
				forGallery.push_back(select);
			}
			res.push_back(forGallery);
		}
		return res;
	}

	void SingleGameManager::SubmitPainting(Model::Painting const* painting)
	{
		selections.push_back(submission = painting);
	}

	Model::Painting const* SingleGameManager::GetSubmission() const
	{
		return submission;
	}

	Model::Painting const* SingleGameManager::GetSelectionForAuction()
	{
		auto idx = cocos2d::RandomHelper::random_int<int>(0, selections.size() - 1);
		auto selection = selections.at(idx);

		selections.erase(selections.begin() + idx);
		return selection;
	}

	std::set<std::string> SingleGameManager::GetPainters() const
	{
		return painters;
	}

	std::unordered_map<std::string, size_t> SingleGameManager::GetReputation() const
	{
		return reputation;
	}

	void SingleGameManager::Bid(const std::pair<int, int>& bid)
	{
		cocos2d::log("[Id %d] bid %d golds.", bid.first, bid.second);

		if (bids.empty() || bids.back().second != bid.second)	// 동시에 들어올 때 조금 대비
		{
			bids.push_back(bid);

			if (newBidEventListener)
				newBidEventListener(bid.second);
		}
	}

	std::vector<std::pair<int, int>> SingleGameManager::GetBids() const
	{
		auto view = bids | std::views::reverse;
		return { view.begin(), view.end() };
	}

	bool SingleGameManager::IsBidUpdated() const
	{
		auto [_, round] = GetCurrentRound();

		return (round == u8"정찰") ? !bids.empty() : false;
	}

	std::pair<int, int> SingleGameManager::GetLastBid() const
	{
		return bids.empty() ? std::make_pair(-1, 0) : bids.back();
	}

	std::pair<int, int> SingleGameManager::GetWinningBid()
	{
		auto [_, round] = GetCurrentRound();

		if (round == u8"비공개 입찰")
			return GetClosedWinningBid();

		auto winningBid = GetLastBid();
		bids.clear();
		return winningBid;
	}

	bool SingleGameManager::HasAllUserSubmitted() const
	{
		return (selections.size() == nPlayers);
	}

	std::pair<int, int> SingleGameManager::GetClosedWinningBid()
	{
		auto winningBid = std::make_pair(0, 0);
		do
		{
			auto bid = bids.back();

			std::erase_if(bids, [=](auto pair)
				{
					return (pair.first == bid.first);
				});

			if (bid.second > winningBid.second)
				winningBid = bid;
		} while (!bids.empty());

		return winningBid;
	}
}
