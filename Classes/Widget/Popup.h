#pragma once
#include <ui/uibutton.h>
#include <ui/uilayout.h>
#include <ui/uilistview.h>


namespace lhs::widget
{
    class Popup : public cocos2d::ui::Layout
    {
    public:
        Popup();

        ~Popup() final = default;

        CREATE_FUNC(Popup);

        bool init() final;

        void EnableCancel() noexcept;

        void SetScale(cocos2d::Vec2 scale) noexcept;

        void AddContent(cocos2d::ui::Widget* widget) noexcept;

    private:
        cocos2d::ui::ListView* contents;
        cocos2d::ui::Button* buttonOk;
        cocos2d::ui::Button* buttonCancel;
    };
}
