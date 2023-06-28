#include "PaintingSubmissionScene.h"
#include "AuctionScene.h"

#include <AudioEngine.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>
using namespace lhs;

#include <Manager/SingleGameManager.h>
using namespace lhs::manager;

#include <Widget/AlignedVBox.h>
#include <Widget/Painting.h>
using namespace lhs::widget;


Scene* PaintingSubmission::createScene()
{
    return PaintingSubmission::create();
}

bool PaintingSubmission::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    const auto origin = Director::getInstance()->getVisibleOrigin();
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // 2. Create and initialize items
    //
    setColor(Color3B::BLACK);

    auto data = SingleGameManager::Instance().GetSubmission();

    auto vbox = Utility::Create<AlignedVBox>();
    vbox->setPosition(visibleSize / 2);
    vbox->setAnchorPoint({ .5f, .5f });

    auto painting = Utility::Create<Painting>();
    painting->SetImage(data);
    painting->setScale(3.f);

    auto painter = Utility::CreateWithFile<Text>(data.painter, lhs::FONT_SIZE_LARGE);
    auto title = Utility::CreateWithFile<Text>(data.title, lhs::FONT_SIZE_LARGE);

    // 3. Add items to scene
    //
    vbox->AddWidget(painter);
    vbox->AddWidget(painting);
    vbox->AddWidget(title);
    addChild(vbox);

    auto changeScene = [](float f)
    {
        if (SingleGameManager::Instance().HasAllUserSubmitted())
        {
            auto scene = Auction::createScene();
            auto transition = TransitionSlideInB::create(.3f, scene);
            Director::getInstance()->replaceScene(transition);
        }
    };
    schedule(changeScene, 3.f, 30, 0, "key");

    return true;
}
