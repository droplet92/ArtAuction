#include "RankingResultScene.h"
#include "WaitingRoomScene.h"

#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* RankingResult::createScene()
{
    return RankingResult::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool RankingResult::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 2. Write your codes below

    auto sprite = Sprite::create("backgrounds/6.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/6.jpg'");
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

            auto label = Label::createWithTTF("Gallery Ranking", "fonts/Dovemayo_gothic.ttf", 40);
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
        auto btn1 = Button::create("Namecard.png", "NamecardPressed.png");
        auto btn2 = Button::create("Namecard.png", "NamecardPressed.png");
        auto btn3 = Button::create("Namecard.png", "NamecardPressed.png");
        auto btn4 = Button::create("Namecard.png", "NamecardPressed.png");

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

    auto okButton = Button::create("StartButton.png", "StartButtonPressed.png");

    okButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            if (type != ui::Widget::TouchEventType::ENDED)
                return;

            auto scene = WaitingRoom::createScene();

            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        });

    auto buttons = ListView::create();
    buttons->setDirection(ScrollView::Direction::HORIZONTAL);
    buttons->setPosition(Vec2{ origin.x + visibleSize.width * 0.75f,
                                origin.y + visibleSize.height * 0.12f });
    buttons->setContentSize(Size{ 500, 500 });
    buttons->setAnchorPoint(Vec2{ 0.4f, 0.5f });
    buttons->addChild(okButton);
    buttons->setItemsMargin(30.f);

    this->addChild(buttons, 1);

    return true;
}


void RankingResult::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
