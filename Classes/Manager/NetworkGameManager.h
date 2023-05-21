#pragma once

#include <Manager/GameManager.h>

namespace lhs::Manager
{
	class NetworkGameManager : public GameManager
	{
	public:
		~NetworkGameManager() final;

		static NetworkGameManager& Instance();

		void Init();

		void SetNumberOfPlayers(size_t nPlayers) final;
		
		std::u8string GetNextRound() final;

		std::vector<std::vector<Model::Painting*>> GetPaintings(size_t nPlayers) const final;

		void SubmitPainting(Model::Painting const* painting) final;

		Model::Painting const* GetSubmission() const final;

		Model::Painting const* GetSelectionForAuction() final;

		std::set<std::string> GetPainters() const;

		std::unordered_map<std::string, size_t> GetReputation() const;

		void Bid(const std::pair<int, int>& bid) final;

		std::vector<std::pair<int, int>> GetBids() const final;

		std::pair<int, int> GetWinningBid() final;

		bool HasAllUserSubmitted() const final;

	private:
		NetworkGameManager();
	};
}