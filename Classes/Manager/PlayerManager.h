#pragma once
#include <Model/Player.h>


namespace lhs::manager
{
	/**
	* The player manager
	*
	* A manager class for players' data.
	* Ths Singleton pattern is used.
	*/
	class PlayerManager
	{
	public:
		static PlayerManager& Instance() noexcept;

		// Return a player whose ID equals to the given id.
		// If there is no player with the given id, return nullptr.
		model::Player* GetPlayer(uint32_t id);

		std::vector<model::Player*> GetRoomPlayers(uint32_t roomId);

		void AddPlayer(std::string_view name);

	private:
		PlayerManager() = default;

		std::vector<model::Player> players;
	};
}
