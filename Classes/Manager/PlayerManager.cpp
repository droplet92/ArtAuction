#include <Manager/PlayerManager.h>

#include <algorithm>

namespace lhs::Manager
{
    PlayerManager& PlayerManager::Instance()
    {
		static PlayerManager* manager = nullptr;

		if (!manager)
			manager = new PlayerManager;
		return *manager;
    }

	// local version
	void PlayerManager::AddPlayer(Player* player)
	{
		static size_t autoIncrementId = 0;

		player->SetId(autoIncrementId++);
		players.push_back(player);
	}
	
	Player* PlayerManager::GetPlayer(size_t id)
	{
		auto player = std::ranges::find_if(players, [id](auto player)
			{
				return player->GetId() == id;
			});

		return (player != std::ranges::end(players))
			? *player
			: nullptr;
	}

	std::vector<lhs::Player*> PlayerManager::GetRoomPlayers(size_t roomId)
	{
		return
		{
			GetPlayer(0),
			GetPlayer(1),
			GetPlayer(2),
			GetPlayer(3)
		};
	}
}