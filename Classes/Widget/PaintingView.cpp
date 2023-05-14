#include "Painting.h"
#include "PaintingView.h"
#include <iostream>
#include <algorithm>
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
	_children.push_back(HBox::create({ 800, 160 }));

	for (auto& painting : paintings)
	{
		painting->addTouchEventListener([&](Ref* ref, Widget::TouchEventType t)
			{
				_selected = dynamic_cast<ui::Painting*>(ref)->getData();
			});

		if (_children.back()->getChildrenCount() == 5)
		{
			_children.push_back(HBox::create({ 800, 160 }));
		}
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
		return _selected;

	return _data.at(RandomHelper::random_int<int>(0, _data.size() - 1));
}
