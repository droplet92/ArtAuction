#include "AuctionedScene.h"
#include "AuctionScene.h"
#include "RankingResultScene.h"

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

    auto title = Label::createWithTTF("Title", "fonts/Dovemayo_wild.ttf", 40);
    if (title == nullptr)
    {
        problemLoading("'fonts/Dovemayo_wild.ttf'");
    }
    else
    {
        // position the title on the center of the screen
        title->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height * 0.8f - title->getContentSize().height));

        // add the title as a child to this layer
        this->addChild(title);
    }

    auto label = Label::createWithTTF("Title", "fonts/Dovemayo_wild.ttf", 40);
    if (label == nullptr)
    {
        problemLoading("'fonts/Dovemayo_wild.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width *.75f,
            origin.y + visibleSize.height * 0.5f + label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label);
    }

    auto artistLabel = Label::createWithTTF("Artist", "fonts/Dovemayo_wild.ttf", 40);
    if (artistLabel == nullptr)
    {
        problemLoading("'fonts/Dovemayo_wild.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        artistLabel->setPosition(Vec2(origin.x + visibleSize.width * .75f,
            origin.y + visibleSize.height * 0.5f - artistLabel->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(artistLabel);
    }

    std::stringstream ss{};
    ss << "droplet92" << ", Congrats!";

    auto msg = Label::createWithTTF(ss.str(), "fonts/Dovemayo_wild.ttf", 40);
    if (msg == nullptr)
    {
        problemLoading("'fonts/Dovemayo_wild.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        msg->setPosition(Vec2(origin.x + visibleSize.width /2,
            origin.y + visibleSize.height * 0.25f + msg->getContentSize().height * 2));

        // add the label as a child to this layer
        this->addChild(msg);
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
