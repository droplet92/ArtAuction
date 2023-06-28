#include "RoomTitle.h"
USING_NS_CC;

#include <Utility.h>


namespace lhs::widget
{
    RoomTitle::RoomTitle()
        : titleLabel(nullptr)
    {
    }

    bool RoomTitle::initWithSpriteFrameName(const std::string& filename)
    {
        if (!Sprite::initWithSpriteFrameName(filename))
            return false;

        titleLabel = Utility::CreateWithFile<Label>("", lhs::FONT_SIZE_LARGE);
        titleLabel->setPosition(Vec2{ 0, -15 } + getContentSize() / 2);
        titleLabel->setAnchorPoint({ .5f, .5f });
        titleLabel->setTextColor(Color4B::BLACK);
        addChild(titleLabel);
        return true;
    }

    void RoomTitle::SetString(const std::string& title) noexcept
    {
        titleLabel->setString(title);
    }
}
