#include "Player.h"
#include <typeinfo>

using namespace lhs;

Player::Player(std::string name)
	: id(0)
	, name(name)
	, money(100)
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

std::vector<Model::Painting*> lhs::Player::GetPaintings() const
{
	return paintings;
}

size_t Player::GetId() const
{
	return id;
}

Model::Painting* lhs::Player::GetPainting(size_t idx) const
{
	return paintings.at(idx);
}
