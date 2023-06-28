#pragma once
#include <map>

#include <Model/Painting.h>


namespace lhs::manager
{
	/**
	* The Game manager interface
	* 
	* This interface is for the SingleGameManager and NetworkGameManager.
	*/
	class GameManager
	{
	public:
		virtual ~GameManager() = default;

		virtual void Init() = 0;

		virtual bool IsBidUpdated() const = 0;

		virtual bool IsRoundEnd() const = 0;

		virtual bool HasAllUserSubmitted() const = 0;

		virtual std::pair<int, std::u8string> GetCurrentRound() const = 0;

		virtual std::vector<std::string> GetPainters() const = 0;

		virtual std::vector<std::vector<model::Painting>> GetPaintings(uint32_t nPlayers) const = 0;

		virtual const model::Painting& GetSubmission() const = 0;

		virtual std::map<std::string, uint32_t> GetReputation() const = 0;

		virtual model::Painting GetSelectionForAuction() = 0;

		virtual std::vector<std::pair<int, int>> GetBids() const = 0;

		virtual std::pair<int, int> GetLastBid() const = 0;

		virtual std::pair<int, int> GetWinningBid() = 0;

		virtual void SetNumberOfPlayers(uint32_t nPlayers) = 0;

		virtual void SubmitPainting(const model::Painting& painting) = 0;

		virtual void Bid(const std::pair<int, int>& bid) = 0;

		virtual void MoveToNextRound() = 0;
	};
}