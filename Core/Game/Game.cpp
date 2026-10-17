#include "Game.h"

#include <algorithm>
#include <cassert>
#include <ranges>
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

	bool Game::Submit(PlayerId player, const Painting& painting)
	{
		if (view.phase != Phase::Submission || view.submissions.at(player))
			return false;

		auto& owner = view.players.at(player);
		const auto gallery = owner.GetPaintings();
		if (std::ranges::find(gallery, painting) == gallery.end())
			return false;

		owner.RemovePainting(painting);
		view.submissions.at(player) = painting;
		events.push_back(PaintingSubmitted{ player });

		// Show the submissions for a moment, then begin the auctions.
		if (HasAllSubmitted())
			view.timeLeft = timings.submissionReveal;
		return true;
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
			BeginSubmission();
			break;
		case Phase::Submission:
			if (!HasAllSubmitted())
			{
				SubmitForTheLate();
				view.timeLeft = timings.submissionReveal;
			}
			break;
		default:
			break;
		}
	}

	void Game::BeginSubmission()
	{
		view.submissions.assign(view.players.size(), std::nullopt);
		Enter(Phase::Submission, timings.submission);
	}

	bool Game::HasAllSubmitted() const
	{
		return std::ranges::all_of(view.submissions, [](const auto& submission) { return submission.has_value(); });
	}

	void Game::SubmitForTheLate()
	{
		// TODO: A player who runs out of time puts up a random painting. Decide whether this is intended.
		for (PlayerId id = 0; id < view.players.size(); ++id)
		{
			if (view.submissions.at(id))
				continue;

			const auto gallery = view.players.at(id).GetPaintings();
			assert(!gallery.empty() && "a player must have a painting to submit.");
			Submit(id, gallery.at(RandomInt(random, 0, static_cast<int>(gallery.size()) - 1)));
		}
	}
}
