#pragma once
#include <concepts>
#include <format>
#include <ranges>

#include <AudioEngine.h>
#include <cocos2d.h>
#include <ui/UIButton.h>
#include <ui/UIListView.h>

#include <Utility.h>
#include <Widget/Popup.h>


namespace lhs::widget
{
    /**
    * The List view of nameplates
    * 
    * It contains nameplate buttons.
    */
    class NameplateList : public cocos2d::ui::ListView
    {
    public:
        NameplateList();

        virtual ~NameplateList() = default;

        CREATE_FUNC(NameplateList);
        
        bool init() final;

        void RegisterPopup(Popup* popup) noexcept;

        template <class T>
            requires std::ranges::range<T> && std::same_as<std::ranges::range_value_t<T>, std::string>
        void AddNames(const T& names)
        {
            auto onButtonPressed = [=](cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
            {
                if (type != cocos2d::ui::Widget::TouchEventType::ENDED)
                    return;

                cocos2d::AudioEngine::play2d("audios/click.mp3");

                if (!popup || popup->isVisible())
                    return;

                auto caller = dynamic_cast<cocos2d::ui::Button*>(ref);
                auto message = std::format("Kick this player [{}]?", caller->getTitleText());

                auto text = Utility::CreateWithFile<cocos2d::ui::Text>(message);
                text->setTextColor(cocos2d::Color4B::BLACK);

                popup->AddContent(text);
                popup->setVisible(true);
            };
            for (const auto name : names)
            {
                auto namecard = Utility::CreateWithSprite<cocos2d::ui::Button>("Namecard.png", "NamecardPressed.png");
                namecard->setTitleFontName(DEFAULT_FONT_NAME);
                namecard->setTitleFontSize(FONT_SIZE_LARGE);
                namecard->setTitleText(name);
                namecard->addTouchEventListener(onButtonPressed);
                addChild(namecard);
            }
        }

    private:
        Popup* popup;
    };
}
