#pragma once

#include <Manager/GameManager.h>

namespace lhs::Manager
{
	class SingleGameManager : public GameManager
	{
	public:
		~SingleGameManager() final;

		static SingleGameManager& Instance();

		void SetNumberOfPlayers(size_t nPlayers) final;

		std::vector<std::vector<Model::Painting*>> GetPaintings(size_t nPlayers) const final;

		void SubmitPainting(Model::Painting const* painting) final;

		Model::Painting const* GetSubmission() const final;

		Model::Painting const* GetSelectionForAuction() final;

		bool HasAllUserSubmitted() const final;

	private:
		SingleGameManager();

		static size_t const MAX_USER_COUNT;

		size_t nPlayers;

		std::vector<Model::Painting const*> selections;

		Model::Painting const* submission;
	};
}