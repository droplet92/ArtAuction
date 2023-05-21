#include "RoomListScene.h"
#include "WaitingRoomScene.h"

#include <AudioEngine.h>
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace ui;


Scene* RoomList::createScene()
{
    return RoomList::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool RoomList::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    auto sprite = Sprite::create("backgrounds/3.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/3.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, -1);
    }

    auto roomTitle = Sprite::createWithSpriteFrameName("RoomTitle.png");
    if (roomTitle == nullptr)
    {
        problemLoading("'RoomTitle.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        {
            roomTitle->setPosition(Vec2{ origin.x + visibleSize.width / 4,
                                origin.y + visibleSize.height * 1.13f - roomTitle->getContentSize().height });

            // add the sprite as a child to this layer
            this->addChild(roomTitle, -1);

            auto label = Label::createWithTTF("Rooms", "fonts/Dovemayo_gothic.ttf", 40);
            if (label == nullptr)
            {
                problemLoading("'fonts/Dovemayo_gothic.ttf'");
            }
            else
            {
                label->setAnchorPoint(Vec2{ 0.5, 0.5 });
                label->setTextColor(Color4B::BLACK);
                label->setPosition(Vec2{ roomTitle->getContentSize().width / 2, roomTitle->getContentSize().height / 2 - 15 });
                roomTitle->addChild(label);
            }
        }
    }

    auto list = ListView::create();
    if (list != nullptr)
    {
        auto btn1 = Button::create("Namecard.png", "NamecardPressed.png", "", Widget::TextureResType::PLIST);
        auto btn2 = Button::create("Namecard.png", "NamecardPressed.png", "", Widget::TextureResType::PLIST);
        auto btn3 = Button::create("Namecard.png", "NamecardPressed.png", "", Widget::TextureResType::PLIST);
        auto btn4 = Button::create("Namecard.png", "NamecardPressed.png", "", Widget::TextureResType::PLIST);

        class PopupStatus
        {
        public:
            bool isOpen() const
            {
                return _isOpen;
            }

            void setOpen(bool doOpen)
            {
                _isOpen = doOpen;
            }

        private:
            bool _isOpen = false;
        };

        auto stat = new PopupStatus{};

        auto func = [=](Ref* sender, Widget::TouchEventType type)
        {
            if (type != Widget::TouchEventType::ENDED)
                return;

            if (stat->isOpen())
                return;

            stat->setOpen(true);

            std::stringstream messagefmt{};
            Button* caller = (Button*)sender;

            messagefmt << "Join this room [" << caller->getTitleText() << "]?";

            auto text = Text::create();
            text->setFontName("fonts/Dovemayo_gothic.ttf");
            text->setFontSize(24);
            text->setTextColor(Color4B::BLACK);
            text->setString(messagefmt.str());

            auto popupOk = Button::create("PopupOk.png", "", "", Widget::TextureResType::PLIST);
            auto popupCancel = Button::create("PopupCancel.png", "", "", Widget::TextureResType::PLIST);

            popupOk->setTitleFontName("fonts/Dovemayo_gothic.ttf");
            popupOk->setTitleFontSize(24);
            popupOk->setTitleText("Yes");
            popupOk->addTouchEventListener([=](auto sender, auto type)
                {
                    this->removeChildByTag(0xDEADBEEF);
                    stat->setOpen(false);
                });
            popupCancel->addTouchEventListener([=](auto sender, auto type)
                {
                    this->removeChildByTag(0xDEADBEEF);
                    stat->setOpen(false);
                });

            auto popup = ListView::create();
            popup->setBackGroundImage("popup.png", Widget::TextureResType::PLIST);
            popup->setBackGroundImageScale9Enabled(true);
            popup->setPosition(Vec2{ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });
            popup->setAnchorPoint(Vec2{ .5f, .5f });

            popup->setContentSize(Size{ popup->getBackGroundImageTextureSize().width,
                                        popup->getBackGroundImageTextureSize().height / 2 });
            popup->setScrollBarEnabled(false);
            popup->setItemsMargin(10.f);
            popup->setPadding(20.f, 40.f, 20.f, 40.f);

            popup->addChild(text);

            popupOk->setAnchorPoint(Vec2{ .5f, 0 });
            popupCancel->setAnchorPoint(Vec2{ .5f, .5f });
            popupOk->setPosition(Vec2{ popup->getPosition().x,
                                        popup->getPosition().y - popup->getInnerContainerSize().height / 2 + 10.f });
            popupCancel->setPosition(Vec2{ popup->getPosition().x + popup->getInnerContainerSize().width / 2 - 20.f,
                                            popup->getPosition().y + popup->getInnerContainerSize().height / 2 - 20.f });

            auto layout = Layout::create();
            layout->addChild(popup);
            layout->addChild(popupOk);
            layout->addChild(popupCancel);

            this->addChild(layout, 0, 0xDEADBEEF);
        };
        btn1->addTouchEventListener(func);
        btn2->addTouchEventListener(func);
        btn3->addTouchEventListener(func);
        btn4->addTouchEventListener(func);

        btn1->setTitleFontName("fonts/Dovemayo_gothic.ttf");
        btn2->setTitleFontName("fonts/Dovemayo_gothic.ttf");
        btn3->setTitleFontName("fonts/Dovemayo_gothic.ttf");
        btn4->setTitleFontName("fonts/Dovemayo_gothic.ttf");
        btn1->setTitleFontSize(40.f);
        btn2->setTitleFontSize(40.f);
        btn3->setTitleFontSize(40.f);
        btn4->setTitleFontSize(40.f);
        btn1->setTitleText("RoomA 2/4");
        btn2->setTitleText("RoomB 1/3");
        btn3->setTitleText("RoomC 1/5");
        btn4->setTitleText("RoomD 3/3");

        list->setContentSize(Size{ 437.5, 400 });
        list->setScrollBarEnabled(false);
        list->setAnchorPoint(Vec2{ 0.5, 0.5 });
        list->setPosition(Vec2{ origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2 });

        list->addChild(btn1);
        list->addChild(btn2);
        list->addChild(btn3);
        list->addChild(btn4);

        this->addChild(list);
    }

    auto readyButton = Button::create("ReadyButton.png", "ReadyButtonPressed.png", "", Widget::TextureResType::PLIST);
    auto startButton = Button::create("StartButton.png", "StartButtonPressed.png", "", Widget::TextureResType::PLIST);

    startButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            if (type != ui::Widget::TouchEventType::ENDED)
                return;

            auto scene = WaitingRoom::createScene();

            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        });

    auto buttons = ListView::create();
    buttons->setDirection(ScrollView::Direction::VERTICAL);
    buttons->setPosition(Vec2{ origin.x + visibleSize.width * 0.9f,
                                origin.y + visibleSize.height * 0.5f });
    buttons->setContentSize(Size{ 300, visibleSize.height });
    buttons->setAnchorPoint(Vec2{ 0.4f, 0.5f });
    buttons->addChild(readyButton);
    buttons->addChild(startButton);
    buttons->setItemsMargin(30.f);

    buttons->getInnerContainer()->setAnchorPoint(Vec2{ 0.5f, 0.5f });

    this->addChild(buttons, 1);

    return true;
}


void RoomList::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
