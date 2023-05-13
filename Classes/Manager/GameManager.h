#pragma once

#include <vector>
#include <Model/Painting.h>

namespace lhs::Manager
{
	class GameManager
	{
	public:
		virtual ~GameManager() = default;

		virtual void SetNumberOfPlayers(size_t nPlayers) = 0;

		virtual std::vector<std::vector<Model::Painting*>> GetPaintings(size_t nPlayers) const = 0;

		virtual void SubmitPainting(Model::Painting const* painting) = 0;

		virtual Model::Painting const* GetSubmission() const = 0;

		virtual bool HasAllUserSubmitted() const = 0;
	};
}