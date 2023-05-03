#include "PaintingSubmissionScene.h"
#include "AuctionScene.h"

USING_NS_CC;

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

    this->setColor(Color3B::BLACK);

    auto label = Label::createWithTTF("Title", "fonts/Dovemayo_wild.ttf", 40);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height * 0.75f - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


    auto msg = Label::createWithTTF("Artist", "fonts/Dovemayo_wild.ttf", 40);
    if (msg == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        msg->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height * 0.25f + msg->getContentSize().height * 2));

        // add the label as a child to this layer
        this->addChild(msg, 1);
    }

    auto moveToNextScene = [](float f)
    {
        auto scene = Auction::createScene();

        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
    };
    this->scheduleOnce(moveToNextScene, 1.f, "asdf");

    return true;
}


void PaintingSubmission::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
