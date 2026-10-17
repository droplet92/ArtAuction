#pragma once
#include <random>


namespace lhs::game
{
	// The engine every game rule draws from. The owner seeds it, so tests can fix the outcome.
	using Random = std::mt19937;

	// Return a uniformly distributed integer in [min, max].
	inline int RandomInt(Random& random, int min, int max)
	{
		return std::uniform_int_distribution<int>{ min, max }(random);
	}
}
