#include "TitleScene.h"
#include "PlayMenuScene.h"

USING_NS_CC;

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
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Title::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    auto title = Sprite::create("backgrounds/GameTitle.png");
    if (title == nullptr)
    {
        problemLoading("'backgrounds/GameTitle.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        title->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

        this->addChild(title);
    }


    auto msg = Label::createWithTTF("- Press to Start -", "fonts/Dovemayo_gothic.ttf", 40);
    if (msg == nullptr)
    {
        problemLoading("'fonts/Dovemayo_gothic.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        msg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + msg->getContentSize().height * 2));

        this->addChild(msg);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("backgrounds/1.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/1.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        this->addChild(sprite, -1);
    }

    auto director = Director::getInstance();
    auto mouse_listener = EventListenerMouse::create();
    auto keyboard_listener = EventListenerKeyboard::create();

    mouse_listener->onMouseDown = [](auto _) {
        auto scene = PlayMenu::createScene();
        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        return true;
    };
    keyboard_listener->onKeyReleased = [](auto _, auto c) {
        auto scene = PlayMenu::createScene();
        Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        return true;
    };

    auto dispatcher = director->getEventDispatcher();

    dispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyboard_listener, this);

    return true;
}


void Title::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
