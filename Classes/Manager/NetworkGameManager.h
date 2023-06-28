#pragma once
#include <Manager/GameManager.h>


namespace lhs::manager
{
	/**
	* The network game manager
	*
	* A Multi-play game implementation of the GameManager.
	* Ths Singleton pattern is used.
	*/
	class NetworkGameManager : public GameManager
	{
	public:
		~NetworkGameManager() final = default;

		[[deprecated("Not implemented yet.")]]
		static NetworkGameManager& Instance();

		void Init();

		bool IsBidUpdated() const final;

		bool IsRoundEnd() const final;

		bool HasAllUserSubmitted() const final;

		std::pair<int, std::u8string> GetCurrentRound() const final;

		std::vector<std::string> GetPainters() const;

		std::vector<std::vector<model::Painting>> GetPaintings(uint32_t nPlayers) const final;

		const model::Painting& GetSubmission() const final;

		std::map<std::string, uint32_t> GetReputation() const;

		model::Painting GetSelectionForAuction() final;

		std::vector<std::pair<int, int>> GetBids() const final;

		std::pair<int, int> GetLastBid() const final;

		std::pair<int, int> GetWinningBid() final;

		void SetNumberOfPlayers(uint32_t nPlayers) final;

		void SubmitPainting(const model::Painting& painting) final;

		void Bid(const std::pair<int, int>& bid) final;

		void MoveToNextRound() final;

	private:
		NetworkGameManager() = default;
	};
}