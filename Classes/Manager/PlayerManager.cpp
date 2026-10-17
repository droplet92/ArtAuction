#include "PlayerManager.h"
using namespace lhs::model;

#include <algorithm>


namespace lhs::manager
{
    PlayerManager& PlayerManager::Instance() noexcept
    {
		static PlayerManager manager{};
		return manager;
    }

	Player* PlayerManager::GetPlayer(uint32_t id)
	{
		auto player = std::ranges::find_if(players, [id](auto player)
			{
				return player.GetId() == id;
			});
		return (player != std::ranges::end(players))
			? &*player	// found
			: nullptr;	// not found
	}

	// singleplay implementation
	std::vector<Player*> PlayerManager::GetRoomPlayers(uint32_t roomId)
	{
		return
		{
			GetPlayer(0),
			GetPlayer(1),
			GetPlayer(2),
			GetPlayer(3)
		};
	}

	// singleplay implementation
	void PlayerManager::AddPlayer(std::string_view name)
	{
		players.push_back({ nextId++, name.data()});
	}

	void PlayerManager::ClearPlayers() noexcept
	{
		players.clear();
		nextId = 0;
	}
}