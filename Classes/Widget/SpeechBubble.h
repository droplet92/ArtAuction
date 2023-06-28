#pragma once
#include <cocos2d.h>
#include <ui/UIScale9Sprite.h>
#include <ui/UIText.h>


namespace lhs::widget
{
    class SpeechBubble : public cocos2d::ui::Scale9Sprite
    {
    public:
        SpeechBubble();

        virtual ~SpeechBubble() = default;

        CREATE_FUNC(SpeechBubble);
        
        bool initWithSpriteFrameName(const std::string& filename) final;

        void SetString(const std::string& words) noexcept;

        void SetFixed(bool isFixed) noexcept;

    private:
        cocos2d::ui::Text* text;    // text to speech
        bool isFixedSize;           // is fixed or scalable
    };
}
