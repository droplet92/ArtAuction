#include "Popup.h"

#include <AudioEngine.h>
#include <CCDirector.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>


namespace lhs::widget
{
	Popup::Popup()
		: contents(nullptr)
        , buttonOk(nullptr)
        , buttonCancel(nullptr)
	{
		setTouchEnabled(true);
	}

    bool Popup::init()
    {
        if (!Layout::init())
            return false;

        const auto visibleSize = Director::getInstance()->getVisibleSize();
        const auto origin = Director::getInstance()->getVisibleOrigin();

        setBackGroundImage("popup.png", Widget::TextureResType::PLIST);
        setBackGroundImageScale9Enabled(true);

        setPosition(origin + visibleSize / 2);
        setAnchorPoint({ .5f, .5f });

        contents = Utility::Create<ListView>();
        contents->setScrollBarEnabled(false);
        contents->setItemsMargin(10.f);
        contents->setLeftPadding(20.f);
        contents->setTopPadding(20.f);

        // 수정필요
        auto onButtonPressed = [=](Ref* sender, auto type)
        {
            contents->removeChildByTag(0xDEADBEEF);
            setVisible(false);
        };
        buttonOk = Utility::CreateWithSprite<Button>("PopupOk.png");
        buttonOk->setTitleFontName("fonts/Dovemayo_gothic.ttf");
        buttonOk->setTitleFontSize(24);
        buttonOk->setTitleText("Yes");
        buttonOk->addTouchEventListener([=](auto, auto) { setEnabled(false); });
        buttonOk->setAnchorPoint({ .5f, 0 });
        buttonOk->addTouchEventListener(onButtonPressed);

        addChild(contents);
        addChild(buttonOk);

        return true;
    }

    void Popup::EnableCancel() noexcept
    {
        auto onButtonPressed = [=](Ref* sender, auto type)
        {
            contents->removeChildByTag(0xDEADBEEF);
            setVisible(false);
        };
        buttonCancel = Utility::CreateWithSprite<Button>("PopupCancel.png");
        buttonCancel->addTouchEventListener([=](auto, auto) { setEnabled(false); });
        buttonCancel->setAnchorPoint({ .5f, .5f });
        buttonCancel->addTouchEventListener(onButtonPressed);

        addChild(buttonCancel);
        contents->setTopPadding(40.f);
    }

    void Popup::SetScale(Vec2 scale) noexcept
    {
        setContentSize({ getBackGroundImageTextureSize().width * scale.x,
                         getBackGroundImageTextureSize().height * scale.y });
        contents->setContentSize(getContentSize());
        buttonOk->setPosition({ getContentSize().width / 2, 10.f });
        if (buttonCancel)
            buttonCancel->setPosition(-Vec2{ 20.f, 20.f } + getContentSize());
    }

    void Popup::AddContent(Widget* widget) noexcept
    {
        contents->addChild(widget, 1, 0xDEADBEEF);
    }
}
