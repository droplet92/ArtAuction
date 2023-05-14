#include "AuctionScene.h"
#include "RankingResultScene.h"

#include <cocos/ui/CocosGUI.h>

#include <Widget/Timer.h>
#include <Widget/Painting.h>
#include <Manager/SingleGameManager.h>

USING_NS_CC;

int Auction::nPlayer = 2;

Scene* Auction::createScene()
{
    return Auction::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

Sprite* MakeCharacter(int n, const Vec2& origin, float visibleWidth)
{
    std::stringstream ss;
    ss << "characters/character" << n << ".png";

    if (auto character = Sprite::create(ss.str()))
    {
        character->setPosition({ origin.x + visibleWidth / 10 * (n * 2 - 1), origin.y });
        character->setAnchorPoint({ .5f, 0 });

        return character;
    }
    return nullptr;
}


// on "init" you need to initialize your instance
bool Auction::init()
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

    if (auto background = Sprite::create("backgrounds/AuctionBackground.png"))
    {
        background->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });
        background->setAnchorPoint({ .5f, .5f });

        addChild(background);
    }

    addChild(MakeCharacter(1, origin, visibleSize.width));
    addChild(MakeCharacter(2, origin, visibleSize.width));
    addChild(MakeCharacter(3, origin, visibleSize.width));
    addChild(MakeCharacter(4, origin, visibleSize.width));
    addChild(MakeCharacter(5, origin, visibleSize.width));

    if (auto sprite = Sprite::create("easel.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 3 });
        sprite->setAnchorPoint({ .5f, .5f });

        addChild(sprite);

        auto selection = lhs::Manager::SingleGameManager::Instance().GetSelectionForAuction();
        if (auto painting = ui::Painting::create(selection))
        {
            painting->setPosition({ sprite->getPositionX(), sprite->getPositionY() });
            painting->setAnchorPoint({ .5f, 0 });
            painting->setScale(1.5f);

            addChild(painting);
        }
    }

    if (auto sprite = Sprite::create("dealer.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 });
        sprite->setAnchorPoint({ .5f, .5f });

        addChild(sprite);
    }

    if (auto sprite = Sprite::create("bubble.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width * .6f, origin.y + visibleSize.height / 3 });
        sprite->setAnchorPoint({ .5f, 0 });

        addChild(sprite);
    }

    if (auto sprite = Sprite::create("ScoreBoardRow.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 8, origin.y + visibleSize.height - sprite->getContentSize().height });
        sprite->setAnchorPoint({ .5f, .1f });

        addChild(sprite);
    }

    if (auto sprite = Sprite::create("ScoreBoardRow.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width * 3 / 10, origin.y + visibleSize.height - sprite->getContentSize().height });
        sprite->setAnchorPoint({ .5f, .1f });

        addChild(sprite);
    }

    if (auto sprite = Sprite::create("ScoreBoardColumn.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width * 6 / 7, origin.y + visibleSize.height - sprite->getContentSize().height });
        sprite->setAnchorPoint({ .5f, .1f });

        addChild(sprite);
    }

    auto timer = ui::Timer::create(30.f);

    timer->setPosition({ visibleSize.width * .5f, visibleSize.height * .85f });
    timer->setAlarm([&]()
        {
            if (--nPlayer < 0)
            {
                auto scene = RankingResult::createScene();

                Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
            }
            else
            {
                // ÆË¾÷ Ç¥½Ã
                removeChildByTag(0xDEADBEEF);
            }

        });
    timer->play();
    addChild(timer, 0, 0xDEADBEEF);

    for (auto n : { 1,2,3,4,5 })
    {
        if (auto board = Sprite::create("BidBoard.png"))
        {
            board->setPosition({ origin.x + visibleSize.width / 10 * (n * 2 - 1), origin.y + visibleSize.height / 6 });
            board->setAnchorPoint({ .5f, 0 });

            //addChild(board);
        }
    }

    // Bid board
    auto layout = ui::Layout::create();
    layout->setBackGroundImage("Popup.png");
    layout->setBackGroundImageScale9Enabled(true);
    layout->setAnchorPoint({ .5f, .5f });
    layout->setContentSize({ layout->getBackGroundImageTextureSize().width * .6f,
                                layout->getBackGroundImageTextureSize().height / 3 });
    layout->setPosition({ visibleSize.width * 6 / 7, visibleSize.height / 2 });

    auto chatfield = ui::TextField::create("Bid yourself", "fonts/Dovemayo_gothic.ttf", 24);
    chatfield->setPosition({ 10.f, layout->getContentSize().height / 2 });
    chatfield->setAnchorPoint({ 0, .5f });
    chatfield->setCursorEnabled(true);
    chatfield->setTextColor(Color4B::BLACK);
    chatfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
    chatfield->addEventListener([](Ref* ref, ui::TextField::EventType type) {
        auto field = (ui::TextField*)ref;
        switch (type)
        {
        case ui::TextField::EventType::ATTACH_WITH_IME: [[fallthrough]];
        case ui::TextField::EventType::DETACH_WITH_IME:
            field->setString("");
            break;
        case ui::TextField::EventType::INSERT_TEXT: [[fallthrough]];
        case ui::TextField::EventType::DELETE_BACKWARD:
            break;
        }
        });

    auto popupOk = ui::Button::create("PopupOk.png");
    popupOk->setTitleFontName("fonts/Dovemayo_gothic.ttf");
    popupOk->setTitleFontSize(24);
    popupOk->setTitleText("Bid");
    popupOk->addTouchEventListener([=](auto _, ui::Widget::TouchEventType type)
        {
            if (type == ui::Widget::TouchEventType::BEGAN)
            {
                log("%s", chatfield->getString().c_str());
                chatfield->setString("");
            }
        });
    popupOk->setAnchorPoint({ .1f, .5f });
    popupOk->setPosition({ layout->getContentSize().width - popupOk->getContentSize().width,
                            layout->getContentSize().height / 2 });

    layout->addChild(chatfield);
    layout->addChild(popupOk);
    addChild(layout);

    return true;
}


void Auction::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
