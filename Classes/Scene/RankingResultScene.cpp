#include "RankingResultScene.h"
#include "WaitingRoomScene.h"

#include <algorithm>
#include <ranges>

#include <AudioEngine.h>
#include <ui/CocosGUI.h>

#include <Manager/PlayerManager.h>

USING_NS_CC;
using namespace ui;


Scene* RankingResult::createScene()
{
    return RankingResult::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool RankingResult::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 2. Write your codes below
    auto players = lhs::Manager::PlayerManager::Instance().GetRoomPlayers(0);

    std::ranges::stable_sort(players, [](lhs::Player* lhs, lhs::Player* rhs)
        {
            return lhs->GetGold() > rhs->GetGold();
        });
    if (auto sprite = Sprite::create("backgrounds/6.jpg"))
    {
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        addChild(sprite, -1);
    }
    if (auto roomTitle = Sprite::createWithSpriteFrameName("RoomTitle.png"))
    {
        roomTitle->setPosition(Vec2{ origin.x + visibleSize.width / 4,
                            origin.y + visibleSize.height * 1.13f - roomTitle->getContentSize().height });

        addChild(roomTitle, -1);

        if (auto label = Label::createWithTTF("Gallery Ranking", "fonts/Dovemayo_gothic.ttf", 40))
        {
            label->setAnchorPoint({ .5f, .5f });
            label->setTextColor(Color4B::BLACK);
            label->setPosition(-Vec2{ 0, 15 } + (roomTitle->getContentSize() / 2));
            roomTitle->addChild(label);
        }
    }
    if (auto list = ListView::create())
    {
        for (const auto& player : players)
        {
            auto namePlate = Button::create();

            namePlate->loadTextureNormal("Namecard.png", Widget::TextureResType::PLIST);
            namePlate->loadTextureDisabled("NamecardPressed.png", Widget::TextureResType::PLIST);
            namePlate->setTitleFontName("fonts/Dovemayo_gothic.ttf");
            namePlate->setTitleFontSize(40.f);
            namePlate->setTitleText(player->GetName());

            list->addChild(namePlate);
        }
        list->setContentSize({ 437.5, 400 });
        list->setScrollBarEnabled(false);
        list->setAnchorPoint({ 0.5, 0.5 });
        list->setPosition({ origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2 });

        addChild(list);
    }
    if (auto okButton = Button::create())
    {
        okButton->loadTextureNormal("OkButton.png", Widget::TextureResType::PLIST);
        okButton->loadTexturePressed("OkButtonPressed.png", Widget::TextureResType::PLIST);
        okButton->setPosition({ origin.x + visibleSize.width / 2, origin.x + visibleSize.height * .12f });
        okButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
            {
                if (type != ui::Widget::TouchEventType::ENDED)
                    return;

                AudioEngine::play2d("audios/click.mp3");

                auto scene = WaitingRoom::createScene();

                Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
            });
        addChild(okButton);
    }
    return true;
}


void RankingResult::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
