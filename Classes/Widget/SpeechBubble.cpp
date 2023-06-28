#include "SpeechBubble.h"
USING_NS_CC;
using namespace ui;

#include <Utility.h>


namespace lhs::widget
{
    SpeechBubble::SpeechBubble()
        : text(nullptr)
        , isFixedSize(false)
    {
    }

    bool SpeechBubble::initWithSpriteFrameName(const std::string& filename)
    {
        if (!Sprite::initWithSpriteFrameName(filename))
            return false;

        // default size is image size of filename
        setContentSize(getTextureRect().size);

        text = Utility::CreateWithFile<Text>("");
        text->setAnchorPoint({ .5f, .5f });
        text->setTextColor(Color4B::BLACK);

        addChild(text);

        return true;
    }

    void SpeechBubble::SetString(const std::string& words) noexcept
    {
        text->setString(words);

        // If scalable, fit content size to the text.
        if (!isFixedSize)
            setContentSize(text->getContentSize() + Size{ 20.f, 20.f });

        text->setPosition(getContentSize() / 2);    // center alignment
    }

    void SpeechBubble::SetFixed(bool isFixed) noexcept
    {
        isFixedSize = isFixed;
    }
}
