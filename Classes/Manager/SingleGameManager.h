#pragma once
#include <functional>

#include <Manager/GameManager.h>


namespace lhs::manager
{
	/**
	* The single game manager
	*
	* A Singleplay game implementation of the GameManager.
	* The Singleton pattern is used.
	*/
	class SingleGameManager : public GameManager
	{
	public:
		~SingleGameManager() final = default;

		static SingleGameManager& Instance();

		void Init() final;

		bool IsBidUpdated() const final;

		bool IsRoundEnd() const final;

		bool HasAllUserSubmitted() const final;

		std::pair<int, std::u8string> GetCurrentRound() const final;

		std::vector<std::string> GetPainters() const final;

		std::vector<std::vector<model::Painting>> GetPaintings(uint32_t nPlayers) const final;

		const model::Painting& GetSubmission() const final;

		std::map<std::string, uint32_t> GetReputation() const final;

		model::Painting GetSelectionForAuction() final;

		std::vector<std::pair<int, int>> GetBids() const final;

		std::pair<int, int> GetLastBid() const final;

		std::pair<int, int> GetWinningBid() final;

		void SetNumberOfPlayers(uint32_t nPlayers) final;

		void SubmitPainting(const model::Painting& painting) final;

		void Bid(const std::pair<int, int>& bid) final;

		void MoveToNextRound() final;

		void AddNewBidEventListener(std::function<void(const std::pair<int, int>&)> listener) noexcept;

		void AddWinningBid(std::string_view painter, int winningBid);

		void UpdateReputation();

		uint32_t GetRealTimeNftPrice();

	private:
		SingleGameManager();

		std::pair<int, int> GetClosedWinningBid();

	private:
		static uint32_t const MAX_USER_COUNT;

		uint32_t nPlayers;

		std::vector<std::u8string> rounds;

		std::vector<model::Painting> selections;

		std::optional<model::Painting> submission;

		std::vector<model::Painting> paintings;

		std::vector<std::string> painters;

		std::map<std::string, uint32_t> reputation;

		std::vector<std::pair<int, int>> bids;

		int prevBidsCount;

		std::function<void(const std::pair<int, int>&)> newBidEventListener;

		// total price of the sold paintings by painter at a round
		// the list of the pair of (painter, price)
		std::vector<std::pair<std::string, int>> winningBids;
	};
}