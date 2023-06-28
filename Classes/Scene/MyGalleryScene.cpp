#include "MyGalleryScene.h"
#include "PaintingSubmissionScene.h"

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

#include <Widget/PaintingView.h>
#include <Widget/Timer.h>
#include <Widget/NameplateList.h>
#include <Widget/RoomTitle.h>
using namespace widget;


static void ChangeScene(const model::Painting& submission, Player* player)
{
    SingleGameManager::Instance().SubmitPainting(submission);
    player->RemovePainting(submission);

    auto scene = PaintingSubmission::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
}

Scene* MyGallery::createScene()
{
    return MyGallery::create();
}

// on "init" you need to initialize your instance
bool MyGallery::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    const auto roomPlayers = PlayerManager::Instance().GetRoomPlayers(0);
    SingleGameManager::Instance().SubmitPainting(roomPlayers[1]->GetPainting(1));
    SingleGameManager::Instance().SubmitPainting(roomPlayers[2]->GetPainting(2));
    SingleGameManager::Instance().SubmitPainting(roomPlayers[3]->GetPainting(3));

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/5.jpg");
    background->setPosition(origin + visibleSize / 2);

    auto roomTitle = Utility::CreateWithSprite<RoomTitle>("RoomTitle.png");
    roomTitle->setPosition(origin + Vec2{ visibleSize.width / 4, visibleSize.height * 1.13f - roomTitle->getContentSize().height });
    roomTitle->SetString("My Gallery");

    auto board = Utility::Create<PaintingView>();
    board->setPosition(visibleSize * .4f);
    board->setAnchorPoint({ .5f, .5f });
    board->setContentSize({ 950, 570 });
    board->setBackGroundImage("GalleryBoard.png", Widget::TextureResType::PLIST);
    board->setBackGroundImageScale9Enabled(true);

    auto view = roomPlayers[0]->GetPaintings()
        | std::views::transform([](auto data)
            {
                auto painting = Utility::Create<widget::Painting>();
                painting->SetImage(data);
                return painting;
            });
    board->AddPaintings({ std::begin(view), std::end(view) });

    auto timer = Utility::Create<widget::Timer>();
    timer->setPosition({ visibleSize.width * .85f, visibleSize.height * .85f });
    timer->SetRemainTime(30);
    timer->SetAlarm([=]()
        {
            ChangeScene(board->GetSelected(), roomPlayers[0]);
        });
    timer->Start();

    auto startButton = Utility::CreateWithSprite<Button>("StartButton.png", "StartButtonPressed.png");
    startButton->setPosition(origin + Vec2{ visibleSize.width * 0.85f, visibleSize.height * 0.12f });
    startButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
        {
            if (type != Widget::TouchEventType::ENDED)
                return;

            AudioEngine::play2d("audios/click.mp3");
            ChangeScene(board->GetSelected(), roomPlayers[0]);
        });

    // 3. Add items to scene
    //
    addChild(background, -1);
    addChild(roomTitle);
    addChild(board);
    addChild(timer);
    addChild(startButton);

    return true;
}
