#include "PaintingView.h"

#include <algorithm>
#include <iostream>

#include <AudioEngine.h>
#include <ccRandom.h>

USING_NS_CC;
using namespace cocos2d::ui;


PaintingView::PaintingView()
	: _selected(nullptr)
{
}

PaintingView::~PaintingView()
{
}

bool PaintingView::init()
{
	if (!VBox::init())
		return false;

	return true;
}

void PaintingView::AddPaintings(std::vector<ui::Painting*> paintings)
{
	_children.push_back(HBox::create({ 950, 190 }));
	
	for (auto& painting : paintings)
	{
		painting->addTouchEventListener([&](Ref* ref, Widget::TouchEventType type)
			{
				if (type != Widget::TouchEventType::ENDED)
					return;

				if (_selected)
					_selected->setColor(Color3B::WHITE);

				_selected = dynamic_cast<ui::Painting*>(ref);

				AudioEngine::play2d("audios/click.mp3");
				_selected->setColor(Color3B::ORANGE);
			});
		if (_children.back()->getChildrenCount() == 5)
			_children.push_back(HBox::create({ 950, 190 }));

		_children.back()->addChild(painting);
		_data.push_back(painting->getData());
	}

	for (const auto& row : _children)
	{
		addChild(row);
	}

}

lhs::Model::Painting const* ui::PaintingView::getSelected() const
{
	if (_selected)
		return _selected->getData();

	return _data.at(RandomHelper::random_int<int>(0, _data.size() - 1));
}
