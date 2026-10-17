#pragma once
#include <optional>
#include <variant>
#include <vector>

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

	// It comes right before the phase changes to Closed.
	struct AuctionClosed
	{
		model::Bid winningBid;
		model::Painting painting;
		std::optional<model::Gold> nftPrice;	// the gold the NFT sold for, in an NFT round
		std::vector<model::Bid> bids;			// from the latest to the earliest
	};

	using GameEvent = std::variant<PhaseChanged, PaintingSubmitted, BidPlaced, AuctionClosed>;
}
