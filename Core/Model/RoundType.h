#pragma once
#include <string_view>


namespace lhs::model
{
	enum class RoundType
	{
		OpenBidding,
		ClosedBidding,
		FixedPrice,
		NftOpenBidding,
	};

	// Return the display name of the round type.
	constexpr std::u8string_view ToString(RoundType type) noexcept
	{
		switch (type)
		{
		case RoundType::OpenBidding:	return u8"Open Bidding";
		case RoundType::ClosedBidding:	return u8"Closed Bidding";
		case RoundType::FixedPrice:		return u8"Fixed Price";
		case RoundType::NftOpenBidding:	return u8"NFT Open Bidding";
		}
		return {};
	}
}
