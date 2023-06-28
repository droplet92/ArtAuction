#include "TitleScene.h"
#include "PlayMenuScene.h"

#include <AudioEngine.h>
USING_NS_CC;

#include <Utility.h>
using namespace lhs;


bool ChangeScene()
{
    auto scene = PlayMenu::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
    return true;
};

Scene* Title::createScene()
{
    return Title::create();
}

bool Title::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    const auto origin = Director::getInstance()->getVisibleOrigin();
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/1.jpg");
    background->setPosition(origin + visibleSize / 2);

    auto title = Utility::CreateWithFile<Sprite>("backgrounds/GameTitle.png");
    title->setPosition(origin + visibleSize / 2);

    auto message = Utility::CreateWithFile<Label>("Press to Start", lhs::FONT_SIZE_LARGE);
    message->setPosition(origin + Vec2{ visibleSize.width / 2, message->getContentSize().height * 2 });
    message->enableOutline(Color4B::BLACK, 3);

    // 3. Add items to scene
    //
    addChild(background, -1);
    addChild(title);
    addChild(message);

    // 4. Add event listeners
    //
    auto mouseEventListener = EventListenerMouse::create();
    mouseEventListener->onMouseDown = [](auto)
    {
        return ChangeScene();
    };
    auto keyboardEventListener = EventListenerKeyboard::create();
    keyboardEventListener->onKeyReleased = [](auto, auto)
    {
        return ChangeScene();
    };
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(mouseEventListener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

    return true;
}
