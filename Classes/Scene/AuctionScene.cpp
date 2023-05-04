#include "AuctionScene.h"
#include "AuctionedScene.h"

USING_NS_CC;

Scene* Auction::createScene()
{
    return Auction::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Auction::init()
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

    auto sp = Sprite::create("Timer.png");
    sp->setContentSize(sp->getContentSize() / 3);

    if (sp != nullptr)
    {
        auto time = Label::createWithTTF("30", "fonts/Dovemayo_gothic.ttf", 60);
        time->setTextColor(Color4B::BLACK);
        time->setAnchorPoint(Vec2{ .5f, .4f });
        time->setPosition(Vec2{ time->getContentSize().width * 1.f, time->getContentSize().height * 0.8f });

        auto timer = ProgressTimer::create(sp);
        timer->setType(ProgressTimer::Type::RADIAL);
        timer->setReverseDirection(true);
        timer->setAnchorPoint(Vec2{ .5f, .5f });
        timer->setPosition(Vec2{ origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7f });

        timer->addChild(time);
        timer->runAction(ProgressTo::create(30, 100));
        timer->schedule([time](float dt)
            {
                static int remainTime = 30;

                if (!remainTime)
                {
                    auto scene = Auctioned::createScene();

                    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
                    return;
                }
                time->setString(std::to_string(--remainTime));
            }, 1.0f, 30, 0, "updateTime");

        auto timerBase = Sprite::create("TimerBase.png");
        timerBase->setAnchorPoint(timer->getAnchorPoint());
        timerBase->setPosition(timer->getPosition());
        timerBase->setContentSize(timerBase->getContentSize() / 3);

        this->addChild(timerBase);
        this->addChild(timer);
    }

    return true;
}


void Auction::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
