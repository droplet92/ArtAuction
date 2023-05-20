#include "TitleScene.h"
#include "PlayMenuScene.h"

#include <audio/include/AudioEngine.h>
#include <Manager/SingleGameManager.h>

USING_NS_CC;


Title::~Title()
{
}

Scene* Title::createScene()
{
    return Title::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Title::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //AudioEngine::preload("audios/bgm1.mp3");
    //AudioEngine::play2d("audios/bgm1.mp3", true);

    /////////////////////////////
    // 2. add your codes below...

    auto sprite = Sprite::create("backgrounds/1.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/1.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition({ visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y });

        addChild(sprite);
    }

    auto title = Sprite::create("backgrounds/GameTitle.png");
    if (title == nullptr)
    {
        problemLoading("'backgrounds/GameTitle.png'");
    }
    else
    {
        title->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });

        addChild(title);
    }


    auto message = Label::createWithTTF("Press to Start", "fonts/Dovemayo_gothic.ttf", 40);
    if (message == nullptr)
    {
        problemLoading("'fonts/Dovemayo_gothic.ttf'");
    }
    else
    {
        message->setPosition({ origin.x + visibleSize.width / 2, origin.y + message->getContentSize().height * 2 });
        //message->enableShadow();
        message->enableOutline(Color4B::BLACK, 3);

        addChild(message);
    }

    auto director = Director::getInstance();
    auto mouse_listener = EventListenerMouse::create();
    auto keyboard_listener = EventListenerKeyboard::create();

    mouse_listener->onMouseDown = [](auto _)
    {
        auto scene = PlayMenu::createScene();
        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        return true;
    };
    keyboard_listener->onKeyReleased = [](auto _, auto c)
    {
        auto scene = PlayMenu::createScene();
        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        return true;
    };

    auto dispatcher = director->getEventDispatcher();

    dispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyboard_listener, this);

    lhs::Manager::SingleGameManager::Instance().Init();

    return true;
}


void Title::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
