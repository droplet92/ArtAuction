#include "NetworkGameManager.h"


namespace lhs::manager
{
	NetworkGameManager& NetworkGameManager::Instance()
	{
		static NetworkGameManager* manager = nullptr;

		if (!manager)
			manager = new NetworkGameManager;
		return *manager;
	}

	void NetworkGameManager::Init()
	{
	}

	bool NetworkGameManager::IsBidUpdated() const
	{
		return false;
	}

	bool NetworkGameManager::IsRoundEnd() const
	{
		return false;
	}

	bool NetworkGameManager::HasAllUserSubmitted() const
	{
		return false;
	}

	std::pair<int, std::u8string>  NetworkGameManager::GetCurrentRound() const
	{
		return {};
	}

	std::vector<std::string> NetworkGameManager::GetPainters() const
	{
		return std::vector<std::string>();
	}

	std::vector<std::vector<model::Painting>> NetworkGameManager::GetPaintings(uint32_t nPlayers) const
	{
		return std::vector<std::vector<model::Painting>>();
	}

	const model::Painting& NetworkGameManager::GetSubmission() const
	{
		return {};
	}

	std::map<std::string, uint32_t> NetworkGameManager::GetReputation() const
	{
		return std::map<std::string, uint32_t>();
	}

	model::Painting NetworkGameManager::GetSelectionForAuction()
	{
		return {};
	}

	std::vector<std::pair<int, int>> NetworkGameManager::GetBids() const
	{
		return std::vector<std::pair<int, int>>();
	}

	std::pair<int, int> NetworkGameManager::GetLastBid() const
	{
		return std::pair<int, int>();
	}

	std::pair<int, int> NetworkGameManager::GetWinningBid()
	{
		return std::pair<int, int>();
	}

	void NetworkGameManager::SetNumberOfPlayers(uint32_t nPlayers)
	{
	}

	void NetworkGameManager::SubmitPainting(const model::Painting& painting)
	{
	}

	void NetworkGameManager::Bid(const std::pair<int, int>& bid)
	{
	}

	void NetworkGameManager::MoveToNextRound()
	{
	}
}
