#include "ui/CocosGUI.h"

#include "MyGalleryScene.h"
#include "PaintingSubmissionScene.h"

USING_NS_CC;
using namespace ui;

Scene* MyGallery::createScene()
{
    return MyGallery::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MyGallery::init()
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

    auto sprite = Sprite::create("backgrounds/5.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/5.jpg'");
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

            auto label = Label::createWithTTF("My Gallery", "fonts/Dovemayo_gothic.ttf", 40);
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

    auto board = Sprite::create("GalleryBoard.png");
    if (board == nullptr)
    {
        problemLoading("'GalleryBoard.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        board->setPosition(Vec2{ origin.x + visibleSize.width * 0.37f, origin.y + visibleSize.height * 0.4f });
        board->setAnchorPoint(Vec2{ 0.5, 0.5 });

        // add the sprite as a child to this layer
        this->addChild(board);
    }

    auto sp = Sprite::create("Timer.png");
    sp->setContentSize(sp->getContentSize() / 2);

    if (sp != nullptr)
    {
        auto time = Label::createWithTTF("30", "fonts/Dovemayo_gothic.ttf", 60);
        time->setTextColor(Color4B::BLACK);
        time->setAnchorPoint(Vec2{ .5f, .4f });
        time->setPosition(Vec2{ time->getContentSize().width * 1.5f, time->getContentSize().height });

        auto timer = ProgressTimer::create(sp);
        timer->setType(ProgressTimer::Type::RADIAL);
        timer->setReverseDirection(true);
        timer->setAnchorPoint(Vec2{ .5f, .5f });
        timer->setPosition(Vec2{ origin.x + visibleSize.width * 0.85f, origin.y + visibleSize.height * 0.8f });

        timer->addChild(time);
        timer->runAction(ProgressTo::create(30, 100));
        timer->schedule([time](float dt)
            {
                static int remainTime = 30;

                if (!remainTime)
                {
                    auto scene = PaintingSubmission::createScene();

                    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
                    return;
                }
                time->setString(std::to_string(--remainTime));
            }, 1.0f, 30, 0, "updateTime");

        auto timerBase = Sprite::create("TimerBase.png");
        timerBase->setAnchorPoint(timer->getAnchorPoint());
        timerBase->setPosition(timer->getPosition());
        timerBase->setContentSize(timerBase->getContentSize() / 2);

        this->addChild(timerBase);
        this->addChild(timer);
    }

    auto startButton = Button::create("StartButton.png", "StartButtonPressed.png");

    startButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
        {
            if (type != ui::Widget::TouchEventType::ENDED)
                return;

            auto scene = PaintingSubmission::createScene();

            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        });
    startButton->setPosition(Vec2{ origin.x + visibleSize.width * 0.85f, origin.y + visibleSize.height * 0.12f });
    this->addChild(startButton);

    return true;
}


void MyGallery::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
