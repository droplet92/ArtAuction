#include "Painting.h"
#include "PaintingView.h"
#include <iostream>

USING_NS_CC;
using namespace cocos2d::ui;

PaintingView::PaintingView()
{
}

PaintingView::~PaintingView()
{
}

bool PaintingView::init()
{
	if (!VBox::init())
		return false;

	_children.push_back(HBox::create({ 800, 160 }));

	try
	{
		std::filesystem::directory_iterator iter{ "../Resources/paintings/Francisco de Goya" };

		for (const auto& entry : iter)
		{
			std::cout << entry.path().u8string() << std::endl;

			auto painting = ui::Painting::create(entry.path().u8string());

			if (_children.back()->getChildrenCount() == 5)
			{
				_children.push_back(HBox::create({ 800, 160 }));
			}
			_children.back()->addChild(painting);
			_data.push_back(painting);
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	for (const auto& row : _children)
	{
		addChild(row);
	}

	return true;
}
