#include "Popup.h"

#include <cocos/ui/CocosGUI.h>
#include <audio/include/AudioEngine.h>
#include <CCDirector.h>

NS_CC_BEGIN


namespace ui
{
	Popup::Popup()
		: contents(nullptr)
        , buttonOk(nullptr)
        , buttonCancel(nullptr)
	{
		setTouchEnabled(true);
	}

	Popup::~Popup()
	{
	}

    Popup* Popup::create(const Vec2& scale, bool canCancel)
    {
        Popup* pRet = new(std::nothrow) Popup;

        if (pRet && pRet->init(scale, canCancel))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
    }

    bool Popup::init(const Vec2& scale, bool canCancel)
    {
        if (!Layout::init())
            return false;

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        setBackGroundImage("popup.png", Widget::TextureResType::PLIST);
        setBackGroundImageScale9Enabled(true);

        setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });
        setAnchorPoint({ .5f, .5f });
        setContentSize({ getBackGroundImageTextureSize().width * scale.x,
                         getBackGroundImageTextureSize().height * scale.y });

        contents = ListView::create();
        {
            contents->setContentSize(getContentSize());
            contents->setScrollBarEnabled(false);
            contents->setItemsMargin(10.f);
            contents->setLeftPadding(20.f);
            contents->setTopPadding(20.f);
        }
        auto onButtonPressed = [=](Ref* sender, auto type)
        {
            contents->removeChildByTag(0xDEADBEEF);
            setVisible(false);
        };
        buttonOk = Button::create();
        {
            buttonOk->loadTextureNormal("PopupOk.png", Widget::TextureResType::PLIST);
            buttonOk->setTitleFontName("fonts/Dovemayo_gothic.ttf");
            buttonOk->setTitleFontSize(24);
            buttonOk->setTitleText("Yes");
            buttonOk->addTouchEventListener([=](auto, auto) { setEnabled(false); });
            buttonOk->setAnchorPoint({ .5f, 0 });
            buttonOk->setPosition({ getContentSize().width / 2, 10.f });
            buttonOk->addTouchEventListener(onButtonPressed);
        }
        addChild(contents);
        addChild(buttonOk);

        if (canCancel)
        {
            buttonCancel = Button::create();
            {
                buttonCancel->loadTextureNormal("PopupCancel.png", Widget::TextureResType::PLIST);
                buttonCancel->addTouchEventListener([=](auto, auto) { setEnabled(false); });
                buttonCancel->setAnchorPoint({ .5f, .5f });
                buttonCancel->setPosition(Vec2(getContentSize()) - Vec2{ 20.f, 20.f });
                buttonCancel->addTouchEventListener(onButtonPressed);
            }
            addChild(buttonCancel);
            contents->setTopPadding(40.f);
        }
        return true;
    }

    void Popup::addContent(Widget* widget)
    {
        contents->addChild(widget, 1, 0xDEADBEEF);
    }
}

NS_CC_END