#pragma once
#include <vector>

#include <Player/Player.h>


namespace lhs::Manager
{
	class PlayerManager
	{
	public:
		static PlayerManager& Instance();

		void AddPlayer(lhs::Player* player);
		
		lhs::Player* GetPlayer(size_t id);

		std::vector<lhs::Player*> GetRoomPlayers(size_t roomId);

	private:
		PlayerManager() = default;

		std::vector<lhs::Player*> players;
	};
}
