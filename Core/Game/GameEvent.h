#pragma once
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

	using GameEvent = std::variant<PhaseChanged, PaintingSubmitted>;
}
