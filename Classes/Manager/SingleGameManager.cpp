#include <Manager/SingleGameManager.h>

#include <iostream>

namespace lhs::Manager
{
	size_t const SingleGameManager::MAX_USER_COUNT = 5;

	SingleGameManager::SingleGameManager()
		: nPlayers(0)
		, submission(nullptr)
	{
		selections.reserve(MAX_USER_COUNT);
	}

	SingleGameManager::~SingleGameManager()
	{
	}

	SingleGameManager& SingleGameManager::Instance()
	{
		static SingleGameManager* manager = nullptr;

		if (!manager)
			manager = new SingleGameManager;
		return *manager;
	}

	void SingleGameManager::SetNumberOfPlayers(size_t nPlayers)
	{
		this->nPlayers = nPlayers;
	}

	std::vector<std::vector<Model::Painting*>> SingleGameManager::GetPaintings(size_t nPlayers) const
	{
		// sqlite ¿¬µ¿
		std::vector<std::vector<Model::Painting*>> res;

		try
		{
			std::filesystem::directory_iterator root{ "../Resources/paintings" };

			for (const auto& directory : root)
			{
				std::filesystem::directory_iterator iter{ directory.path() };
				std::vector<Model::Painting*> paintings;
				std::cout << directory.path().string() << std::endl;

				for (const auto& entry : iter)
				{
					//std::cout << entry.path().string() << std::endl;

					auto painting = new Model::Painting;

					std::stringstream ss{ entry.path().stem().string() };
					painting->path = entry.path();
					std::getline(ss, painting->title, '-');
					std::getline(ss, painting->author, '-');

					paintings.push_back(painting);
				}
				res.push_back(paintings);
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
		return res;
	}

	void SingleGameManager::SubmitPainting(Model::Painting const* painting)
	{
		selections.push_back(submission = painting);
	}

	Model::Painting const* SingleGameManager::GetSubmission() const
	{
		return submission;
	}

	bool SingleGameManager::HasAllUserSubmitted() const
	{
		return (selections.size() == nPlayers);
	}
}
