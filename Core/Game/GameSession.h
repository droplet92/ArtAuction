#pragma once
#include <optional>
#include <vector>

#include <Game/Game.h>
#include <Game/GameEvent.h>
#include <Game/GameView.h>
#include <Model/Bid.h>
#include <Model/Painting.h>


namespace lhs::game
{
	/**
	* A game as one player sees it
	*
	* The screens play through this only: they send the player's commands,
	* pass the time, read the view, and take the events.
	* A local game and an online game will implement it.
	*/
	class GameSession
	{
	public:
		virtual ~GameSession() = default;

		// Return the player this session plays as.
		virtual model::PlayerId GetLocalPlayer() const noexcept = 0;

		virtual void StartGame() = 0;

		virtual bool Submit(const model::Painting& painting) = 0;

		virtual std::optional<BidError> PlaceBid(model::Gold amount) = 0;

		// Let the time pass by dt seconds. Call it every frame.
		virtual void Update(float dt) = 0;

		virtual const GameView& View() const noexcept = 0;

		// Return the events since the last call, in the order they happened.
		[[nodiscard]] virtual std::vector<GameEvent> TakeEvents() = 0;
	};
}
