#pragma once
#include <cocos2d.h>
#include <ui/UIListView.h>
#include <ui/UITextField.h>


namespace lhs::widget
{
    class ChatBox : public cocos2d::Sprite
    {
    public:
        ChatBox();

        virtual ~ChatBox() = default;

        CREATE_FUNC(ChatBox);

        bool initWithSpriteFrameName(const std::string& filename) final;

    private:
        cocos2d::ui::ListView* chatlog;     // previous chat logs
        cocos2d::ui::TextField* chatfield;  // typing field
    };
}
