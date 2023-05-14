#include "Player.h"
#include <typeinfo>
#include <algorithm>
#include <ranges>
#include <string>
#include <set>
#include <map>

#include <Manager/SingleGameManager.h>

using namespace lhs;

Player::Player(std::string name)
	: id(0)
	, name(name)
	, gold(100)
{
}

void Player::SetPaintings(const std::vector<Model::Painting*>& paintings)
{
	this->paintings = paintings;
}

void Player::SetId(size_t id)
{
	this->id = id;
}

std::vector<Model::Painting*> Player::GetPaintings() const
{
	return paintings;
}

size_t Player::GetId() const
{
	return id;
}

Model::Painting* Player::GetPainting(size_t idx) const
{
	return paintings.at(idx);
}

std::map<std::string, size_t> Player::GetInformations() const
{
	std::map<std::string, size_t> information;

	auto category = Manager::SingleGameManager::Instance().GetPainters();
	auto view = paintings
		| std::views::transform([](auto model)
			{
				return model->painter;
			});
	for (const auto& painter : category)
		information.emplace(painter, 0);

	std::ranges::for_each(view, [&](auto author)
		{
			++information[author];
		});
	return information;
}

void Player::AddPainting(Model::Painting* painting)
{
	paintings.push_back(painting);
}

void Player::RemovePainting(Model::Painting const* target)
{
	std::erase_if(paintings, [=](auto source)
		{
			return (source->title == target->title);
		});
}
