#include "PaintingSubmissionScene.h"
#include "AuctionScene.h"
#include "Widget/Painting.h"
#include <Manager/SingleGameManager.h>
#include <audio/include/AudioEngine.h>

USING_NS_CC;

PaintingSubmission::~PaintingSubmission()
{
}

Scene* PaintingSubmission::createScene()
{
    return PaintingSubmission::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

PaintingSubmission* PaintingSubmission::create()
{
    PaintingSubmission* pRet = new(std::nothrow) PaintingSubmission;

    if (pRet && pRet->init())
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

// on "init" you need to initialize your instance
bool PaintingSubmission::init()
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

    setColor(Color3B::BLACK);

    auto data = const_cast<lhs::Model::Painting*>(lhs::Manager::SingleGameManager::Instance().GetSubmission());

    if (auto vbox = ui::VBox::create())
    {
        auto layout = ui::Layout::create();

        auto painting = ui::Painting::create(data);
        auto painter = ui::Text::create(painting->getData()->painter, "fonts/Dovemayo_wild.ttf", 40);
        auto title = ui::Text::create(painting->getData()->title, "fonts/Dovemayo_wild.ttf", 40);

        painting->setScale(3.f);

        float width = painting->getContentSize().width;
        float height = 0;

        {
            painter->setPosition({ width / 2, painter->getContentSize().height / 2 });
            painter->setAnchorPoint({ .5f, .5f });
            
            auto _layout = layout->clone();
            _layout->setContentSize({ width, painter->getContentSize().height });
            _layout->addChild(painter);
            vbox->addChild(_layout);

            height += painter->getContentSize().height;
        }

        {
            painting->setPosition({ width / 2, painting->getContentSize().height / 2 });
            painting->setAnchorPoint({ .5f, .5f });

            auto _layout = layout->clone();
            _layout->setContentSize({ width, painting->getContentSize().height });
            _layout->addChild(painting);
            vbox->addChild(_layout);

            height += painting->getContentSize().height;
        }

        {
            title->setPosition({ width / 2, title->getContentSize().height / 2 });
            title->setAnchorPoint({ .5f, .5f });

            auto _layout = layout->clone();
            _layout->setContentSize({ width, title->getContentSize().height });
            _layout->addChild(title);
            vbox->addChild(_layout);

            height += title->getContentSize().height;
        }
        vbox->setContentSize({ width, height });
        vbox->setPosition({ visibleSize.width / 2, visibleSize.height / 2 });
        vbox->setAnchorPoint({ .5f, .5f });

        addChild(vbox);
    }

    auto moveToNextScene = [](float f)
    {
        if (lhs::Manager::SingleGameManager::Instance().HasAllUserSubmitted())
        {
            auto scene = Auction::createScene();

            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        }
    };
    schedule(moveToNextScene, 3.f, 30, 0, "key");

    return true;
}


void PaintingSubmission::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
