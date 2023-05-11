#include "AuctionedScene.h"
#include "AuctionScene.h"
#include "RankingResultScene.h"

#include "Widget/Painting.h"

USING_NS_CC;

Scene* Auctioned::createScene(bool isEnd)
{
    log("isEnd: %d", isEnd);
    return Auctioned::create(isEnd);
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

Auctioned* Auctioned::create(bool isEnd)
{
    Auctioned* pRet = new(std::nothrow) Auctioned;

    if (pRet && pRet->init(isEnd))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

// on "init" you need to initialize your instance
bool Auctioned::init(bool isEnd)
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
    // 2. 

    this->setColor(Color3B::BLACK);

    if (auto vbox = ui::VBox::create())
    {
        auto layout = ui::Layout::create();
        layout->setContentSize({ 200, 100 });

        if (auto title = ui::Text::create("Title", "fonts/Dovemayo_wild.ttf", 40))
        {
            title->setPosition({ 100,50 });
            title->setAnchorPoint({ .5f, .5f });

            auto _layout = layout->clone();
            //_layout->setContentSize({ 200, title->getContentSize().height });
            _layout->addChild(title);
            vbox->addChild(_layout);
        }

        if (auto painting = ui::Painting::create(R"(F:\workspace\tests\ArtAuction\ArtAuction\Resources\paintings\AI\Midjourney - Chaos (2022).png)"))
        {
            painting->setPosition({ 100,50 });
            painting->setAnchorPoint({ .5f, .5f });

            auto _layout = layout->clone();
            _layout->addChild(painting);
            vbox->addChild(_layout);
        }

        if (auto artist = ui::Text::create("Artist", "fonts/Dovemayo_wild.ttf", 40))
        {
            artist->setPosition({ 100,50 });
            artist->setAnchorPoint({ .5f, .5f });

            auto _layout = layout->clone();
            //_layout->setContentSize({ 200, artist->getContentSize().height });
            _layout->addChild(artist);
            vbox->addChild(_layout);
        }

        std::stringstream ss{};
        ss << "droplet92" << ", Congrats!";

        if (auto message = ui::Text::create(ss.str(), "fonts/Dovemayo_wild.ttf", 40))
        {
            message->setPosition({ 100,50 });
            message->setAnchorPoint({ .5f, .5f });

            auto _layout = layout->clone();
            //_layout->setContentSize({ 200, artist->getContentSize().height });
            _layout->addChild(message);
            vbox->addChild(_layout);
        }
        vbox->setContentSize({ 200, 300 });
        vbox->setPosition({ visibleSize.width / 2, visibleSize.height / 2 });
        //vbox->setBackGroundColor(Color3B::BLUE);
        //vbox->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        vbox->setAnchorPoint({ .5f, .5f });
        addChild(vbox);
    }

    auto moveToNextScene = [=](float f)
    {
        auto scene = isEnd ? RankingResult::createScene() : Auction::createScene();

        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
    };
    this->scheduleOnce(moveToNextScene, 3.f, "asdf");

    return true;
}


void Auctioned::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
