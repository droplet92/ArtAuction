#pragma once

#include <Manager/GameManager.h>

namespace lhs::Manager
{
	class SingleGameManager : public GameManager
	{
	public:
		~SingleGameManager() final;

		static SingleGameManager& Instance();

		void Init() final;

		void SetNumberOfPlayers(size_t nPlayers) final;

		std::vector<std::vector<Model::Painting*>> GetPaintings(size_t nPlayers) const final;

		void SubmitPainting(Model::Painting const* painting) final;

		Model::Painting const* GetSubmission() const final;

		Model::Painting const* GetSelectionForAuction() final;

		std::set<std::string> GetPainters() const final;

		std::unordered_map<std::string, size_t> GetReputation() const final;

		void Bid(const std::pair<int, int>& bid) final;

		std::vector<std::pair<int, int>> GetBids() const final;

		std::pair<int, int> GetWinningBid() final;

		bool HasAllUserSubmitted() const final;

	private:
		SingleGameManager();

		static size_t const MAX_USER_COUNT;

		size_t nPlayers;

		std::vector<Model::Painting const*> selections;

		Model::Painting const* submission;

		std::vector<Model::Painting*> paintings;

		std::set<std::string> painters;

		std::unordered_map<std::string, size_t> reputation;

		std::vector<std::pair<int, int>> bids;
	};
}