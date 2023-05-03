#include "ExplanationScene.h"
#include "MyGalleryScene.h"

USING_NS_CC;

Scene* Explanation::createScene()
{
    return Explanation::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Explanation::init()
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

    this->setColor(Color3B::BLACK);

    auto label = Label::createWithTTF("Round 1", "fonts/Dovemayo_gothic.ttf", 40);
    if (label == nullptr)
    {
        problemLoading("'fonts/Dovemayo_gothic.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        log("position: %lf, %lf", label->getPositionX(), label->getPositionY());
        this->addChild(label);


        std::stringstream mm{};
        mm << u8"이번 경매는 " << u8"비공개 " << u8"경매입니다.";

        auto msg = Label::createWithTTF(mm.str(), "fonts/Dovemayo_gothic.ttf", 24);
        if (msg == nullptr)
        {
            problemLoading("'fonts/Dovemayo_gothic.ttf'");
        }
        else
        {
            // position the label on the center of the screen
            msg->setPosition(Vec2(origin.x + visibleSize.width / 2, label->getPosition().y / 2));

            // add the label as a child to this layer
            log(msg->getString().c_str());
            log("position: %lf, %lf", msg->getPositionX(), msg->getPositionY());
            this->addChild(msg);
        }
    }

    auto moveToNextScene = [](float f)
    {
        auto scene = MyGallery::createScene();

        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
    };
    this->scheduleOnce(moveToNextScene, 1.f, "asdf");

    return true;
}


void Explanation::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
