#pragma once
#include <cstdint>


namespace lhs::model
{
	using PlayerId = uint32_t;
	using Gold = uint32_t;

	class Bid
	{
	public:
		PlayerId bidder;
		Gold amount;

		bool operator==(const Bid& rhs) const = default;
	};
}
