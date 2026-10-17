#pragma once
#include <variant>

#include <Game/GameView.h>


namespace lhs::game
{
	struct PhaseChanged
	{
		Phase phase;
	};

	using GameEvent = std::variant<PhaseChanged>;
}
