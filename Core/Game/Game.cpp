#include "Game.h"

#include <algorithm>
#include <cassert>
#include <utility>

using namespace lhs::model;


namespace lhs::game
{
	Game::Game(Catalog catalog, std::vector<std::string> playerNames, Random random, Timings timings)
		: catalog(std::move(catalog))
		, playerNames(std::move(playerNames))
		, random(std::move(random))
		, timings(timings)
	{
		view.painters = this->catalog.painters;
	}

	void Game::Start()
	{
		assert((view.phase == Phase::Lobby || view.phase == Phase::GameOver) && "a game is already running.");

		// Start a new game from a clean state.
		view.players.clear();
		for (PlayerId id = 0; id < playerNames.size(); ++id)
			view.players.emplace_back(id, playerNames[id]);

		// Deal 10 paintings at random to each player.
		auto copy = catalog.paintings;

		for (auto& player : view.players)
		{
			std::vector<Painting> forGallery;

			for (int i = 0; i < PAINTINGS_PER_PLAYER; i++)
			{
				assert(!copy.empty() && "the catalog must have enough paintings.");
				auto select = copy.at(RandomInt(random, 0, static_cast<int>(copy.size()) - 1));

				std::erase(copy, select);
				forGallery.push_back(select);
			}
			player.SetPaintings(forGallery);
		}

		reputation = Reputation{ catalog.painters };
		view.reputation = reputation.Get();

		// Every round type is played once, in a random order.
		rounds = { RoundType::OpenBidding, RoundType::ClosedBidding, RoundType::FixedPrice, RoundType::NftOpenBidding };
		std::ranges::shuffle(rounds, random);

		view.round = 1;
		view.roundType = rounds.front();
		Enter(Phase::RoundIntro, timings.roundIntro);
	}

	void Game::Update(float dt)
	{
		if (view.timeLeft <= 0.f)
			return;	// The phase does not end by itself.

		view.timeLeft -= dt;

		// A long step may run out more than one phase.
		while (view.timeLeft <= 0.f)
		{
			auto overrun = -view.timeLeft;

			view.timeLeft = 0.f;
			OnTimeOut();

			if (view.timeLeft <= 0.f)
				break;
			view.timeLeft -= overrun;
		}
	}

	const GameView& Game::View() const noexcept
	{
		return view;
	}

	std::vector<GameEvent> Game::TakeEvents()
	{
		return std::exchange(events, {});
	}

	void Game::Enter(Phase phase, float duration)
	{
		view.phase = phase;
		view.timeLeft = duration;
		events.push_back(PhaseChanged{ phase });
	}

	void Game::OnTimeOut()
	{
		switch (view.phase)
		{
		case Phase::RoundIntro:
			Enter(Phase::Submission);
			break;
		default:
			break;
		}
	}
}
