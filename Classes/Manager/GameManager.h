#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <Model/Painting.h>

namespace lhs::Manager
{
	class GameManager
	{
	public:
		virtual ~GameManager() = default;

		virtual void Init() = 0;

		virtual void SetNumberOfPlayers(size_t nPlayers) = 0;

		virtual std::u8string GetNextRound() = 0;

		virtual std::vector<std::vector<Model::Painting*>> GetPaintings(size_t nPlayers) const = 0;

		virtual void SubmitPainting(Model::Painting const* painting) = 0;

		virtual Model::Painting const* GetSubmission() const = 0;

		virtual Model::Painting const* GetSelectionForAuction() = 0;

		virtual std::set<std::string> GetPainters() const = 0;

		virtual std::unordered_map<std::string, size_t> GetReputation() const = 0;

		virtual void Bid(const std::pair<int, int>& bid) = 0;

		virtual std::vector<std::pair<int, int>> GetBids() const = 0;

		virtual std::pair<int, int> GetWinningBid() = 0;

		virtual bool HasAllUserSubmitted() const = 0;
	};
}