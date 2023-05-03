#include "ui/CocosGUI.h"

#include "WaitingRoomScene.h"
#include "ExplanationScene.h"

USING_NS_CC;
using namespace ui;

Scene* WaitingRoom::createScene()
{
    return WaitingRoom::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool WaitingRoom::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    auto sprite = Sprite::create("backgrounds/4.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/4.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, -1);
    }

    auto roomTitle = Sprite::create("RoomTitle.png");
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

            auto label = Label::createWithTTF("Stay in the middle", "fonts/Dovemayo_gothic.ttf", 40);
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

    auto chatbox = Sprite::create("chatbox.png");
    if (chatbox == nullptr)
    {
        problemLoading("'chatbox.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        chatbox->setPosition(Vec2(visibleSize.width * 0.75f + origin.x, visibleSize.height * 0.6f + origin.y));

        // add the sprite as a child to this layer
        this->addChild(chatbox, -1);
    }

    auto list = ListView::create();
    if (list != nullptr)
    {
        auto btn1 = Button::create("Namecard.png", "NamecardPressed.png");
        auto btn2 = Button::create("Namecard.png", "NamecardPressed.png");
        auto btn3 = Button::create("Namecard.png", "NamecardPressed.png");
        auto btn4 = Button::create("Namecard.png", "NamecardPressed.png");

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

                messagefmt << "Kick this player [" << caller->getTitleText() << "]?";

                auto text = Text::create();
                text->setFontName("fonts/Dovemayo_gothic.ttf");
                text->setFontSize(24);
                text->setTextColor(Color4B::BLACK);
                text->setString(messagefmt.str());

                auto popupOk = Button::create("PopupOk.png");
                auto popupCancel = Button::create("PopupCancel.png");

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
                popup->setBackGroundImage("Popup.png");
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
        btn1->setTitleText("droplet9250");
        btn2->setTitleText("loremipsum");
        btn3->setTitleText("pdi0409");
        btn4->setTitleText("hi");

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

    auto chatlog = ListView::create();

    chatlog->setContentSize(Size{ 400, 420 });
    chatlog->setAnchorPoint(Vec2{ 0, 0.5 });
    chatlog->setPosition(Vec2{ origin.x + visibleSize.width * 0.6f, origin.y + visibleSize.height * 0.63f });

    this->addChild(chatlog);

    auto chatfield = TextField::create("Type a message", "fonts/Dovemayo_gothic.ttf", 24);
    chatfield->setPosition(Vec2{ origin.x + chatlog->getPosition().x, origin.y + chatlog->getPosition().y - 240});
    chatfield->setAnchorPoint(Vec2{ 0, 0.5 });
    chatfield->setCursorEnabled(true);
    chatfield->setColor(Color3B::WHITE);
    chatfield->setPlaceHolderColor(Color4B::WHITE);
    chatfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
    chatfield->addEventListener([](Ref* ref, TextField::EventType type) {
        TextField* field = (TextField*)ref;
        switch (type)
        {
        case TextField::EventType::ATTACH_WITH_IME: [[fallthrough]];
        case TextField::EventType::DETACH_WITH_IME:
            field->setString("");
            break;
        case TextField::EventType::INSERT_TEXT: [[fallthrough]];
        case TextField::EventType::DELETE_BACKWARD:
            break;
        }
    });

    this->addChild(chatfield, 2);

    auto readyButton = Button::create("ReadyButton.png", "ReadyButtonPressed.png");
    auto startButton = Button::create("StartButton.png", "StartButtonPressed.png");

    startButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            if (type != ui::Widget::TouchEventType::ENDED)
                return;

            auto scene = Explanation::createScene();

            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        });

    auto buttons = ListView::create();
    buttons->setDirection(ScrollView::Direction::HORIZONTAL);
    buttons->setPosition(Vec2{ origin.x + visibleSize.width * 0.75f,
                                origin.y + visibleSize.height * 0.12f });
    buttons->setContentSize(Size{ 500, 500 });
    buttons->setAnchorPoint(Vec2{ 0.4f, 0.5f });
    buttons->addChild(readyButton);
    buttons->addChild(startButton);
    buttons->setItemsMargin(30.f);

    this->addChild(buttons, 1);

    auto director = Director::getInstance();
    auto keyboard_listener = EventListenerKeyboard::create();

    keyboard_listener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
        if (code == EventKeyboard::KeyCode::KEY_ENTER)
        {
            auto chat = Text::create(chatfield->getString(), "fonts/Dovemayo_gothic.ttf", 24);

            auto sentinel = (chatlog->getChildrenCount() + 1) * chat->getContentSize().height
                > chatlog->getInnerContainerSize().height;

            if (sentinel)
            {
                chatlog->setInnerContainerSize(Size{ chatlog->getContentSize().width,
                                            chatlog->getInnerContainerSize().height + chat->getContentSize().height });
            }
            chatlog->addChild(chat);
            if (sentinel)
                chatlog->jumpToBottom();
            chatfield->setString("");
        }
        return true;
    };

    auto dispatcher = director->getEventDispatcher();

    dispatcher->addEventListenerWithSceneGraphPriority(keyboard_listener, this);

    return true;
}


void WaitingRoom::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
