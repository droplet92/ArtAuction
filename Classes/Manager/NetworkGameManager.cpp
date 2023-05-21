#include "Manager/GameManager.h"
#include "NetworkGameManager.h"

namespace lhs::Manager
{
	NetworkGameManager::NetworkGameManager()
	{

	}

	NetworkGameManager::~NetworkGameManager()
	{
	}

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

	void NetworkGameManager::SetNumberOfPlayers(size_t nPlayers)
	{
	}

	std::u8string NetworkGameManager::GetNextRound()
	{
		return std::u8string();
	}

	std::vector<std::vector<Model::Painting*>> NetworkGameManager::GetPaintings(size_t nPlayers) const
	{
		return std::vector<std::vector<Model::Painting*>>();
	}

	void NetworkGameManager::SubmitPainting(Model::Painting const* painting)
	{
	}

	Model::Painting const* NetworkGameManager::GetSubmission() const
	{
		return nullptr;
	}

	Model::Painting const* NetworkGameManager::GetSelectionForAuction()
	{
		return {};
	}

	std::set<std::string> NetworkGameManager::GetPainters() const
	{
		return std::set<std::string>();
	}

	std::unordered_map<std::string, size_t> NetworkGameManager::GetReputation() const
	{
		return std::unordered_map<std::string, size_t>();
	}

	void NetworkGameManager::Bid(const std::pair<int, int>& bid)
	{
	}

	std::vector<std::pair<int, int>> NetworkGameManager::GetBids() const
	{
		return std::vector<std::pair<int, int>>();
	}

	std::pair<int, int> NetworkGameManager::GetWinningBid()
	{
		return std::pair<int, int>();
	}

	bool NetworkGameManager::HasAllUserSubmitted() const
	{
		return false;
	}

}
