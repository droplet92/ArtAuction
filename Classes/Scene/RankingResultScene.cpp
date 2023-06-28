#include "RankingResultScene.h"
#include "WaitingRoomScene.h"

#include <algorithm>
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

Scene* RankingResult::createScene()
{
    return RankingResult::create();
}

// on "init" you need to initialize your instance
bool RankingResult::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto players = manager::PlayerManager::Instance().GetRoomPlayers(0);

    for (auto& player : players)
        player->SellAll(manager::SingleGameManager::Instance().GetReputation());

    std::ranges::stable_sort(players, [](const Player* lhs, const Player* rhs)
        {
            return lhs->GetGold() > rhs->GetGold();
        });

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/6.jpg");
    background->setPosition(origin + visibleSize / 2);

    auto roomTitle = Utility::CreateWithSprite<RoomTitle>("RoomTitle.png");
    roomTitle->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height * 1.13f - roomTitle->getContentSize().height });
    roomTitle->SetString("Gallery Ranking");

    auto nameplateList = Utility::Create<NameplateList>();
    nameplateList->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height / 2 });
    nameplateList->setAnchorPoint({ .5f, .5f });
    nameplateList->AddNames(players | std::views::transform([](Player* p)
        {
            std::stringstream ss{};
            ss << p->GetName() << " (" << p->GetGold() << ")";
            return ss.str();
        }));

    auto okButton = Utility::CreateWithSprite<Button>("OkButton.png", "OkButtonPressed.png");
    okButton->setPosition(origin + Vec2{ visibleSize.width / 2, visibleSize.height * .12f });
    okButton->addTouchEventListener(ChangeScene);

    // 3. Add items to scene
    //
    addChild(background, -1);
    addChild(roomTitle);
    addChild(nameplateList);
    addChild(okButton);

    return true;
}
