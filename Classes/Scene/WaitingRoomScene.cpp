#include "WaitingRoomScene.h"
#include "ExplanationScene.h"

#include <ranges>

#include <AudioEngine.h>
#include <ui/CocosGUI.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>
using namespace lhs;

#include <Manager/PlayerManager.h>
#include <Manager/SingleGameManager.h>
using namespace lhs::manager;
using namespace lhs::model;

#include <Widget/ChatBox.h>
#include <Widget/NameplateList.h>
#include <Widget/RoomTitle.h>
using namespace lhs::widget;


static void ChangeScene(Ref*, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;

    AudioEngine::play2d("audios/click.mp3");

    auto players = PlayerManager::Instance().GetRoomPlayers(0);
    auto paintings = SingleGameManager::Instance().GetPaintings(4);

    for (auto player : players)
    {
        player->SetPaintings(paintings.back());
        paintings.pop_back();
    }
    auto scene = Explanation::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
}

Scene* WaitingRoom::createScene()
{
    return WaitingRoom::create();
}

bool WaitingRoom::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    const auto origin = Director::getInstance()->getVisibleOrigin();
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    SingleGameManager::Instance().SetNumberOfPlayers(4);
    PlayerManager::Instance().AddPlayer("You");
    PlayerManager::Instance().AddPlayer("Andrew");
    PlayerManager::Instance().AddPlayer("Benjamin");
    PlayerManager::Instance().AddPlayer("Catherine");
    const auto players = PlayerManager::Instance().GetRoomPlayers(0);

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/4.jpg");
    background->setPosition(origin + visibleSize / 2);

    auto roomTitle = Utility::CreateWithSprite<RoomTitle>("RoomTitle.png");
    roomTitle->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height * 1.13f - roomTitle->getContentSize().height });
    roomTitle->SetString("Single Play");

    auto popup = Utility::Create<Popup>();
    popup->SetScale({ 1, .5f });
    popup->setVisible(false);

    auto nameplateList = Utility::Create<NameplateList>();
    nameplateList->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height / 2 });
    nameplateList->setAnchorPoint({ .5f, .5f });
    nameplateList->AddNames(players | std::views::transform([](Player* p) { return p->GetName(); }));
    //nameplateList->RegisterPopup(popup);

    auto chatbox = Utility::CreateWithSprite<ChatBox>("chatbox.png");
    chatbox->setPosition(origin + Vec2{ visibleSize.width * 0.75f, visibleSize.height * 0.6f });

    auto buttons = Utility::Create<ListView>();
    buttons->setPosition(origin + Vec2{ visibleSize.width * 0.75f, visibleSize.height * 0.12f });
    buttons->setAnchorPoint({ .4f, .5f });
    buttons->setContentSize({ 500, 100 });
    buttons->setDirection(ScrollView::Direction::HORIZONTAL);
    buttons->setItemsMargin(30.f);
    buttons->setScrollBarEnabled(false);

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
    addChild(chatbox);
    addChild(buttons);

    return true;
}
