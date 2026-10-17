#pragma once
#include <optional>
#include <variant>

#include <Game/GameView.h>


namespace lhs::game
{
	struct PhaseChanged
	{
		Phase phase;
	};

	struct PaintingSubmitted
	{
		model::PlayerId player;
	};

	struct BidPlaced
	{
		model::PlayerId bidder;
		std::optional<model::Gold> amount;	// empty in a closed bidding round
	};

	using GameEvent = std::variant<PhaseChanged, PaintingSubmitted, BidPlaced>;
}
