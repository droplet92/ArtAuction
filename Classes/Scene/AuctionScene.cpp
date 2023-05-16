#include "AuctionScene.h"
#include "RankingResultScene.h"

#include <cocos/ui/CocosGUI.h>

#include <Widget/Timer.h>
#include <Widget/Painting.h>
#include <Manager/SingleGameManager.h>
#include <Manager/PlayerManager.h>
#include "../Utility.h"


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

    if (auto background = Sprite::create("backgrounds/AuctionBackground.jpg"))
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

    auto selections = lhs::Manager::SingleGameManager::Instance().GetSelectionsForAuction();

    auto easel = Sprite::create("easel.png");
    easel->setPosition({ origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 6 });
    easel->setAnchorPoint({ .5f, 0 });

    addChild(easel);

    if (auto sprite = Sprite::create("dealer.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 });
        sprite->setAnchorPoint({ .5f, .5f });

        addChild(sprite);
    }

    selections.pop_back();
    selections.pop_back();
    selections.pop_back();

    for (const auto& selection : selections)
    {
        if (auto painting = ui::Painting::create(selection))
        {
            painting->setScale(2.5f);
            painting->setAnchorPoint({ .5f, 0 });
            painting->setPosition({ easel->getPositionX() + 10, easel->getPositionY() + easel->getContentSize().height - painting->getBottomPadding() + 10 });

            addChild(painting);
        }

        if (auto artInfo = ui::ListView::create())
        {
            artInfo->setBackGroundImage("ArtInfoBoard.png");
            artInfo->setContentSize(artInfo->getBackGroundImageTextureSize());
            artInfo->setAnchorPoint({ .5f, .1f });
            artInfo->setPosition({ origin.x + visibleSize.width * 6 / 7, origin.y + visibleSize.height - artInfo->getContentSize().height });
            artInfo->setPadding(20, 20, 20, 20);
            artInfo->setItemsMargin(10);

            artInfo->addChild(ui::Text::create(selection->painter, "fonts/Dovemayo_gothic.ttf", 24));
            artInfo->addChild(ui::Text::create(selection->title, "fonts/Dovemayo_gothic.ttf", 16));

            addChild(artInfo);
        }

        auto timer = ui::Timer::create(30);

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
                    // 팝업 표시
                }
            });
        addChild(timer);

        auto bubble = ui::Scale9Sprite::create("bubble.png");
        bubble->setPosition({ origin.x + visibleSize.width * .6f, origin.y + visibleSize.height / 3 });
        bubble->setAnchorPoint({ .5f, 0 });
        bubble->setContentSize(bubble->getTextureRect().size);

        addChild(bubble);

        auto messages = std::vector<std::string>{
            selection->painter + lhs::Utility::ConvertToAscii(u8"\n의 작품입니다."),
            lhs::Utility::ConvertToAscii(u8"10골드부터\n시작합니다."),
            lhs::Utility::ConvertToAscii(u8"입찰\n시작합니다!"),
        };
        auto text = ui::Text::create("", "fonts/Dovemayo_gothic.ttf", 24);
        text->setAnchorPoint({ .5f, .5f });
        text->setTextColor(Color4B::BLACK);
        bubble->addChild(text);
        bubble->setOpacity(0);

        float delay = 2.0f;
        float showDuration = 1.0f;
        float hideDuration = 1.0f;

        auto delayAction = DelayTime::create(delay);

        // Helper 함수: 메시지 표시 액션 시퀀스 생성
        auto createMessageAction = [&](const std::string& message) {
            auto showText = CallFunc::create([=]() {
                text->setString(message);
                bubble->setOpacity(0xFF);
                bubble->setContentSize(text->getContentSize() + Size{ 10.f, 10.f });
                text->setPosition(bubble->getContentSize() / 2);
                });
            auto hideText = CallFunc::create([=]() {
                text->setString("");
                bubble->setOpacity(0);
                bubble->setContentSize(text->getContentSize() + Size{ 10.f, 10.f });
                text->setPosition(bubble->getContentSize() / 2);
                });
            auto showAction = Sequence::create(FadeIn::create(showDuration), nullptr);
            auto hideAction = Sequence::create(DelayTime::create(hideDuration), FadeOut::create(hideDuration), nullptr);

            return Sequence::create(showAction, showText, hideAction, hideText, nullptr);
        };

        // 메시지 표시 시퀀스 생성
        auto messageSequence = createMessageAction(messages[0]); // 첫 번째 메시지는 별도로 생성
        for (int i = 1; i < messages.size(); ++i) {
            auto messageAction = createMessageAction(messages[i]);
            messageSequence = Sequence::create(messageSequence, delayAction->clone(), messageAction, nullptr);
        }

        auto updateTimerAction = CallFunc::create([=]() {
            timer->start();
            });

        auto sequence = Sequence::create(delayAction, messageSequence, updateTimerAction, DelayTime::create(32.f), CallFunc::create([=]() {
            timer->reset(30);
            }), nullptr);

        auto games = Sequence::create(sequence, sequence, nullptr);

        runAction(games);
    }

    auto player = lhs::Manager::PlayerManager::Instance().GetPlayer(0);

    if (auto gallery = ui::ListView::create())
    {
        gallery->setBackGroundImage("PaintersInfoBoard.png");
        gallery->setContentSize(gallery->getBackGroundImageTextureSize());
        gallery->setAnchorPoint({ .5f, .1f });
        gallery->setPosition({ origin.x + visibleSize.width / 8, origin.y + visibleSize.height - gallery->getContentSize().height });
        gallery->setPadding(10, 10, 10, 10);

        auto info = player->GetInformations();
        auto title = ui::Text::create("My Gallery", "fonts/Dovemayo_gothic.ttf", 24);
        
        title->setTextColor({ 0x58, 0xcd, 0xf8, 0xFF });
        gallery->addChild(title);

        for (const auto& datum : info)
        {
            std::stringstream ss;
            ss << datum.first << ": " << datum.second;

            auto text = ui::Text::create(ss.str(), "fonts/Dovemayo_gothic.ttf", 16);

            gallery->addChild(text);
        }
        addChild(gallery);
    }

    if (auto reputation = ui::ListView::create())
    {
        reputation->setBackGroundImage("PaintersInfoBoard.png");
        reputation->setContentSize(reputation->getBackGroundImageTextureSize());
        reputation->setAnchorPoint({ .5f, .1f });
        reputation->setPosition({ origin.x + visibleSize.width * 3 / 10, origin.y + visibleSize.height - reputation->getContentSize().height });
        reputation->setPadding(10, 10, 10, 10);

        auto data = lhs::Manager::SingleGameManager::Instance().GetReputation();
        auto title = ui::Text::create("Reputation", "fonts/Dovemayo_gothic.ttf", 24);

        title->setTextColor({ 0x58, 0xcd, 0xf8, 0xFF });
        reputation->addChild(title);

        for (const auto& datum : data)
        {
            std::stringstream ss;
            ss << datum.first << ": $" << datum.second;

            auto text = ui::Text::create(ss.str(), "fonts/Dovemayo_gothic.ttf", 16);

            reputation->addChild(text);
        }
        addChild(reputation);
    }

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

    std::stringstream ss;
    ss << "Gold: " << player->GetGold();

    auto chatfield = ui::TextField::create(ss.str(), "fonts/Dovemayo_gothic.ttf", 24);
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
                auto offer = chatfield->getString();

                cocos2d::log("%s", offer.c_str());

                if (player->GetGold() < std::stoul(offer))
                {
                    // 팝업 띄우기
                    return;
                }
                chatfield->setPlaceHolder("Offer: " + offer);
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
