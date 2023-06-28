#include "PaintingView.h"

#include <algorithm>

#include <AudioEngine.h>
#include <ccRandom.h>
USING_NS_CC;
using namespace ui;


namespace lhs::widget
{
	static constexpr auto MAX_COUNT_IN_ROW = 5;

	PaintingView::PaintingView()
		: selected(nullptr)
	{
	}

	void PaintingView::AddPaintings(const std::vector<Painting*>& paintings)
	{
		// Add an initial row.
		children.push_back(HBox::create({ 950, 190 }));

		for (auto& painting : paintings)
		{
			painting->addTouchEventListener([&](Ref* ref, Widget::TouchEventType type)
				{
					if (type != Widget::TouchEventType::ENDED)
						return;

					AudioEngine::play2d("audios/click.mp3");

					// change the selected
					if (selected)
						selected->SetColor(Color3B::WHITE);

					selected = dynamic_cast<Painting*>(ref);
					selected->SetColor(Color3B::ORANGE);	// focus on
				});

			// If the current row is full, add an additional row.
			if (children.back()->getChildrenCount() == MAX_COUNT_IN_ROW)
				children.push_back(HBox::create({ 950, 190 }));

			children.back()->addChild(painting);
			data.push_back(painting->GetData());
		}
		for (const auto& row : children)
			addChild(row);
	}

	const model::Painting& PaintingView::GetSelected() const
	{
		if (selected)
			return selected->GetData();

		// If the selected is nullptr, return a random one.
		CCASSERT(!data.empty(), "data must not empty.");
		return data.at(RandomHelper::random_int<int>(0, data.size() - 1));
	}
}
