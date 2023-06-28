#include "RoomListScene.h"
#include "WaitingRoomScene.h"

#include <AudioEngine.h>
#include <ui/CocosGUI.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>
using namespace lhs;

#include <Widget/NameplateList.h>
#include <Widget/RoomTitle.h>
using namespace lhs::widget;


static void ChangeScene(Ref*, Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED)
        return;

    AudioEngine::play2d("audios/click.mp3");

    auto scene = WaitingRoom::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
}

Scene* RoomList::createScene()
{
    return RoomList::create();
}

bool RoomList::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/4.jpg");
    background->setPosition(origin + visibleSize / 2);

    auto roomTitle = Utility::CreateWithSprite<RoomTitle>("RoomTitle.png");
    roomTitle->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height * 1.13f - roomTitle->getContentSize().height });
    roomTitle->SetString("Multiplay");

    auto popup = Utility::Create<Popup>();
    popup->SetScale({ 1, .5f });
    popup->setVisible(false);

    auto nameplateList = Utility::Create<NameplateList>();
    nameplateList->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height / 2 });
    nameplateList->setAnchorPoint({ .5f, .5f });
    nameplateList->AddNames(std::vector<std::string>{ "RoomA 2/4", "RoomB 1/3", "RoomC 1/5", "RoomD 3/3" });
    //nameplateList->RegisterPopup(popup);

    auto buttons = Utility::Create<ListView>();
    buttons->setPosition(origin + Vec2{ visibleSize.width * 0.9f, visibleSize.height * 0.5f });
    buttons->setAnchorPoint({ 0.4f, 0.5f });
    buttons->setContentSize({ 300, visibleSize.height });
    buttons->setDirection(ScrollView::Direction::VERTICAL);
    buttons->setItemsMargin(30.f);
    buttons->getInnerContainer()->setAnchorPoint({ 0.5f, 0.5f });

    auto readyButton = Utility::CreateWithSprite<Button>("ReadyButton.png", "ReadyButtonPressed.png");
    readyButton->setVisible(false);

    auto startButton = Utility::CreateWithSprite<Button>("StartButton.png", "StartButtonPressed.png");
    startButton->addTouchEventListener(ChangeScene);

    buttons->addChild(readyButton);
    buttons->addChild(startButton);

    // 3. Add items to scene
    //
    addChild(background, -1);
    addChild(roomTitle);
    addChild(popup, 1);
    addChild(nameplateList);
    addChild(buttons);

    return true;
}
