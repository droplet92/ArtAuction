#include "ui/CocosGUI.h"
#include "PlayMenuScene.h"
#include "WaitingRoomScene.h"
#include "RoomListScene.h"
#include <audio/include/AudioEngine.h>

USING_NS_CC;
using namespace ui;

PlayMenu::~PlayMenu()
{
}

Scene* PlayMenu::createScene()
{
    return PlayMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PlayMenu::init()
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
    // 2. add your codes below...

    auto singlePlayButton = Button::create();

    singlePlayButton->loadTextureNormal("SinglePlayButton.png", Widget::TextureResType::PLIST);
    singlePlayButton->loadTexturePressed("SinglePlayButtonPressed.png", Widget::TextureResType::PLIST);
    singlePlayButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.66f));
    singlePlayButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto scene = WaitingRoom::createScene();

        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            break;
        case ui::Widget::TouchEventType::ENDED:
            AudioEngine::play2d("audios/click.mp3");
            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
            break;
        default:
            break;
        }
        });
    this->addChild(singlePlayButton, 1);

    auto multiplayButton = Button::create();

    multiplayButton->loadTextureNormal("MultiplayButton.png", Widget::TextureResType::PLIST);
    multiplayButton->loadTexturePressed("MultiplayButtonPressed.png", Widget::TextureResType::PLIST);
    multiplayButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.33f));
    multiplayButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        auto scene = RoomList::createScene();

        switch (type)
        {
        case ui::Widget::TouchEventType::ENDED:
            AudioEngine::play2d("audios/click.mp3");
            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
            break;
        default:
            break;
        }
        });

    multiplayButton->setEnabled(false);

    this->addChild(multiplayButton, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("backgrounds/2.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/2.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

    return true;
}


void PlayMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
