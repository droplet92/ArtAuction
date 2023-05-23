#pragma once
#include <Manager/GameManager.h>
#include <functional>


namespace lhs::Manager
{
	class SingleGameManager : public GameManager
	{
	public:
		~SingleGameManager() final = default;

		static SingleGameManager& Instance();

		void Init() final;

		void SetNumberOfPlayers(size_t nPlayers) final;

		void MoveToNextRound() final;

		std::pair<int, std::u8string> GetCurrentRound() const final;

		std::vector<std::vector<Model::Painting*>> GetPaintings(size_t nPlayers) const final;

		void SubmitPainting(Model::Painting const* painting) final;

		Model::Painting const* GetSubmission() const final;

		Model::Painting const* GetSelectionForAuction() final;

		std::set<std::string> GetPainters() const final;

		std::unordered_map<std::string, size_t> GetReputation() const final;

		void Bid(const std::pair<int, int>& bid) final;

		std::vector<std::pair<int, int>> GetBids() const final;

		bool IsBidUpdated() const final;

		std::pair<int, int> GetLastBid() const final;

		std::pair<int, int> GetWinningBid() final;

		bool HasAllUserSubmitted() const final;

		inline bool IsRoundEnd() const final { return selections.empty(); }

		//
		inline void AddNewBidEventListener(std::function<void(int)> listener) { newBidEventListener = listener; }

		void AddWinningBid(const std::string& painter, int winningBid);

		void UpdateReputation();

		size_t GetRealTimeNftPrice();

	private:
		SingleGameManager();

		std::pair<int, int> GetClosedWinningBid();

		static size_t const MAX_USER_COUNT;

		size_t nPlayers;

		std::vector<std::u8string> rounds;

		std::vector<Model::Painting const*> selections;

		Model::Painting const* submission;

		std::vector<Model::Painting*> paintings;

		std::set<std::string> painters;

		std::unordered_map<std::string, size_t> reputation;

		std::vector<std::pair<int, int>> bids;

		int prevBidsCount;

		//
		std::function<void(int)> newBidEventListener;

		// painter, golds
		std::vector<std::pair<std::string, int>> winningBids;
	};
}