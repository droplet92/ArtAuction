#pragma once
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <Model/Bid.h>
#include <Model/Player.h>
#include <Model/RoundType.h>


namespace lhs::game
{
	enum class Phase
	{
		Lobby,
		RoundIntro,			// the rules of the round are explained
		Submission,			// each player picks a painting to auction
		RoundSettlement,	// the painters' reputation is updated
		GameOver,			// the galleries are sold and ranked
	};

	/**
	* Everything about the game a screen may show
	*/
	struct GameView
	{
		Phase phase = Phase::Lobby;
		float timeLeft = 0.f;										// seconds until the phase ends by itself
		int round = 0;												// from 1 to the number of rounds
		model::RoundType roundType = model::RoundType::OpenBidding;
		std::vector<model::Player> players;							// indexed by the player id
		std::vector<std::string> painters;
		std::map<std::string, uint32_t> reputation;					// painter-(points) table
		std::vector<std::optional<model::Painting>> submissions;	// this round's paintings, indexed by the player id
	};
}
