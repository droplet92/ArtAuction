#include "LocalSession.h"

#include <utility>
#include <variant>

using namespace lhs::model;


namespace lhs::game
{
	static constexpr PlayerId LOCAL_PLAYER = 0;

	LocalSession::LocalSession(Catalog catalog, std::vector<std::string> playerNames, Random random, Timings timings)
		: game(std::move(catalog), playerNames, Random{ random() }, timings)
		, random(std::move(random))
		, submissionBegan(false)
		, biddingBegan(false)
	{
		for (PlayerId id = LOCAL_PLAYER + 1; id < playerNames.size(); ++id)
			computers.emplace_back(id);
	}

	PlayerId LocalSession::GetLocalPlayer() const noexcept
	{
		return LOCAL_PLAYER;
	}

	void LocalSession::StartGame()
	{
		game.Start();
	}

	bool LocalSession::Submit(const Painting& painting)
	{
		return game.Submit(LOCAL_PLAYER, painting);
	}

	std::optional<BidError> LocalSession::PlaceBid(Gold amount)
	{
		return game.PlaceBid({ LOCAL_PLAYER, amount });
	}

	void LocalSession::Update(float dt)
	{
		if (game.View().phase == Phase::Bidding)
		{
			for (auto& computer : computers)
				computer.Update(dt, game, random);
		}

		game.Update(dt);
		CollectEvents();

		// The computer players submit as soon as the gallery opens, before the local player.
		if (std::exchange(submissionBegan, false))
		{
			for (const auto& computer : computers)
				game.Submit(computer.GetId(), computer.ChooseSubmission(game.View().players.at(computer.GetId())));
		}

		if (std::exchange(biddingBegan, false))
		{
			for (auto& computer : computers)
				computer.BeginBidding(game.View().roundType);
		}
	}

	const GameView& LocalSession::View() const noexcept
	{
		return game.View();
	}

	std::vector<GameEvent> LocalSession::TakeEvents()
	{
		CollectEvents();
		return std::exchange(events, {});
	}

	void LocalSession::CollectEvents()
	{
		for (auto& event : game.TakeEvents())
		{
			if (auto changed = std::get_if<PhaseChanged>(&event))
			{
				submissionBegan |= (changed->phase == Phase::Submission);
				biddingBegan |= (changed->phase == Phase::Bidding);
			}
			events.push_back(std::move(event));
		}
	}
}
