#include "AuctionScene.h"
#include "AuctionedScene.h"

#include "Widget/Timer.h"

USING_NS_CC;

int Auction::nPlayer = 2;

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
    // 2. add your codes below...

    auto timer = ui::Timer::create(3.f);

    timer->setPosition(Vec2{ visibleSize.width * .5f, visibleSize.height * .85f });
    timer->setAlarm([&](auto _)
        {
            auto scene = Auctioned::createScene(--nPlayer < 0);

            Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
        });
    this->addChild(timer);

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
