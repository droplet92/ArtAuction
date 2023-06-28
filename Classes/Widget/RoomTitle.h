#pragma once
#include <cocos2d.h>


namespace lhs::widget
{
    class RoomTitle : public cocos2d::Sprite
    {
    public:
        RoomTitle();

        virtual ~RoomTitle() = default;

        CREATE_FUNC(RoomTitle);
        
        bool initWithSpriteFrameName(const std::string& filename) final;

        void SetString(const std::string& title) noexcept;

    private:
        cocos2d::Label* titleLabel;
    };
}
