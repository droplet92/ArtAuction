#include "AuctionScene.h"
#include "RankingResultScene.h"

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

    if (auto background = Sprite::create("backgrounds/AuctionBackground.png"))
    {
        background->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });
        background->setAnchorPoint({ .5f, .5f });

        addChild(background);
    }

    if (auto sprite = Sprite::create("test.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 2, origin.y });
        sprite->setAnchorPoint({ .5f, 0 });

        addChild(sprite);
    }

    if (auto sprite = Sprite::create("easel.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 3 });
        sprite->setAnchorPoint({ .5f, .5f });
        sprite->setScale(0.02f);

        addChild(sprite);
    }

    if (auto sprite = Sprite::create("dealer.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 });
        sprite->setAnchorPoint({ .5f, .5f });

        addChild(sprite);
    }

    auto timer = ui::Timer::create(3.f);

    timer->setPosition({ visibleSize.width * .5f, visibleSize.height * .85f });
    timer->setAlarm([&]()
        {
            if (--nPlayer < 0)
            {
                auto scene = RankingResult::createScene();

                Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
            }
            else
            {
                // ÆË¾÷ Ç¥½Ã
                removeChildByTag(0xDEADBEEF);
            }

        });
    timer->play();
    addChild(timer, 0, 0xDEADBEEF);

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
