#include "AlignedVBox.h"
USING_NS_CC;

#include <Utility.h>


namespace lhs::widget
{
	AlignedVBox::AlignedVBox()
		: width(0.f)
		, height (0.f)
	{
	}

	void AlignedVBox::AddWidget(Widget* widget)
	{
		// update width
		if (width < widget->getContentSize().width)
		{
			width = widget->getContentSize().width;
			auto layouts = getChildren();

			for (auto& layout : layouts)
			{
				auto widget = layout->getChildByTag(0);

				widget->setPosition({ width / 2, widget->getContentSize().height / 2 });
				layout->setContentSize({ width, widget->getContentSize().height });
			}
		}
		// update height
		height += widget->getContentSize().height;

		// update size
		setContentSize({ width, height });

		// add the widget enclosed by a layout
		widget->setPosition({ width / 2, widget->getContentSize().height / 2 });
		widget->setAnchorPoint({ .5f, .5f });

		auto layout = Utility::Create<Layout>();
		layout->setContentSize({ width, widget->getContentSize().height });
		layout->addChild(widget, 0, 0);	// add a tag 0

		addChild(layout);
	}
}
