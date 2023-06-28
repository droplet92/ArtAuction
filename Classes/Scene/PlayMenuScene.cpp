#include "PlayMenuScene.h"
#include "WaitingRoomScene.h"
#include "RoomListScene.h"

#include <AudioEngine.h>
#include <ui/CocosGUI.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>
using namespace lhs;


static void ChangeScene(Ref*, Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        AudioEngine::play2d("audios/click.mp3");

        auto scene = WaitingRoom::createScene();
        auto transition = TransitionSlideInB::create(.3f, scene);
        Director::getInstance()->replaceScene(transition);
    }
}

Scene* PlayMenu::createScene()
{
    return PlayMenu::create();
}

bool PlayMenu::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/2.jpg");
    background->setPosition(origin + visibleSize / 2);

    auto singlePlayButton = Utility::CreateWithSprite<Button>("SinglePlayButton.png", "SinglePlayButtonPressed.png");
    singlePlayButton->setPosition(origin + Vec2{ visibleSize.width / 2, visibleSize.height * 0.66f });
    singlePlayButton->addTouchEventListener(ChangeScene);

    auto multiplayButton = Utility::CreateWithSprite<Button>("MultiplayButton.png", "MultiplayButtonPressed.png");
    multiplayButton->setPosition(origin + Vec2{ visibleSize.width / 2, visibleSize.height * 0.33f });
    multiplayButton->addTouchEventListener(ChangeScene);
    multiplayButton->setEnabled(false);

    // 3. Add items to scene
    //
    addChild(background, -1);
    addChild(singlePlayButton);
    addChild(multiplayButton);

    return true;
}
