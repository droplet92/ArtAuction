#include "AuctionScene.h"
#include "RankingResultScene.h"

#include <cocos/ui/CocosGUI.h>

#include <Widget/Timer.h>
#include <Widget/Painting.h>
#include <Manager/SingleGameManager.h>
#include <Manager/PlayerManager.h>
#include "../Utility.h"

#include <ccRandom.h>

USING_NS_CC;


constexpr auto fontBasic = "fonts/Dovemayo_gothic.ttf";
constexpr auto fontSizeMedium = 24;
constexpr auto fontSizeSmall = 16;

constexpr float delay = 2.0f;
constexpr float showDuration = 1.0f;
constexpr float hideDuration = 1.0f;


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

    auto player = lhs::Manager::PlayerManager::Instance().GetPlayer(0);


    if (auto background = Sprite::create("backgrounds/AuctionBackground.jpg"))
    {
        background->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });
        background->setAnchorPoint({ .5f, .5f });

        addChild(background);
    }

    auto easel = Sprite::create("easel.png");
    {
        easel->setPosition({ origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 6 });
        easel->setAnchorPoint({ .5f, 0 });

        addChild(easel);
    }

    if (auto sprite = Sprite::create("dealer.png"))
    {
        sprite->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 });
        sprite->setAnchorPoint({ .5f, .5f });

        addChild(sprite);
    }
    addChild(MakeCharacter(1, origin, visibleSize.width));
    addChild(MakeCharacter(2, origin, visibleSize.width));
    addChild(MakeCharacter(3, origin, visibleSize.width));
    addChild(MakeCharacter(4, origin, visibleSize.width));
    addChild(MakeCharacter(5, origin, visibleSize.width));

    if (auto reputation = ui::ListView::create())
    {
        reputation->setBackGroundImage("PaintersInfoBoard.png");
        reputation->setContentSize(reputation->getBackGroundImageTextureSize());
        reputation->setAnchorPoint({ .5f, .1f });
        reputation->setPosition({ origin.x + visibleSize.width * 3 / 10, origin.y + visibleSize.height - reputation->getContentSize().height });
        reputation->setPadding(10, 10, 10, 10);

        auto data = lhs::Manager::SingleGameManager::Instance().GetReputation();
        auto title = ui::Text::create("Reputation", fontBasic, fontSizeMedium);

        title->setTextColor({ 0x58, 0xcd, 0xf8, 0xFF });
        reputation->addChild(title);

        for (const auto& datum : data)
        {
            std::stringstream ss;
            ss << datum.first << ": $" << datum.second;

            auto text = ui::Text::create(ss.str(), fontBasic, fontSizeSmall);

            reputation->addChild(text);
        }
        addChild(reputation);
    }

    auto bidBoard = ui::Layout::create();
    {
        bidBoard->setBackGroundImage("Popup.png");
        bidBoard->setBackGroundImageScale9Enabled(true);
        bidBoard->setAnchorPoint({ .5f, .5f });
        bidBoard->setContentSize({ bidBoard->getBackGroundImageTextureSize().width * .6f,
                                    bidBoard->getBackGroundImageTextureSize().height / 3 });
        bidBoard->setPosition({ visibleSize.width * 6 / 7, visibleSize.height / 2 });

        addChild(bidBoard);
    }

    std::stringstream ss{};
    ss << "Gold: " << player->GetGold();

    auto bidField = ui::TextField::create(ss.str(), fontBasic, fontSizeMedium);
    {
        bidField->setPosition({ 10.f, bidBoard->getContentSize().height / 2 });
        bidField->setAnchorPoint({ 0, .5f });
        bidField->setCursorEnabled(true);
        bidField->setTextColor(Color4B::BLACK);
        bidField->setTextHorizontalAlignment(TextHAlignment::LEFT);
        bidField->addEventListener([](Ref* ref, ui::TextField::EventType type) {
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

        bidBoard->addChild(bidField);
    }

    auto bidButton = ui::Button::create("PopupOk.png");
    {
        bidButton->setTitleFontName(fontBasic);
        bidButton->setTitleFontSize(fontSizeMedium);
        bidButton->setTitleText("Bid");
        bidButton->addTouchEventListener([=](auto _, ui::Widget::TouchEventType type)
            {
                if (type == ui::Widget::TouchEventType::BEGAN)
                {
                    auto offer = bidField->getString();

                    cocos2d::log("%s", offer.c_str());

                    if (player->GetGold() < std::stoul(offer))
                    {
                        // �˾� ����
                        return;
                    }
                    lhs::Manager::SingleGameManager::Instance().Bid({ 0, std::stoi(offer) });
                    bidField->setPlaceHolder("Offer: " + offer);
                    bidField->setString("");
                }
            });
        bidButton->setAnchorPoint({ .1f, .5f });
        bidButton->setPosition({ bidBoard->getContentSize().width - bidButton->getContentSize().width,
                                bidBoard->getContentSize().height / 2 });

        bidBoard->addChild(bidButton);
    }

    auto timer = ui::Timer::create(30);
    {
        timer->setPosition({ visibleSize.width * .5f, visibleSize.height * .85f });
        timer->setAlarm([]() {});

        addChild(timer);
    }

    auto bubble = ui::Scale9Sprite::create("bubble.png");
    {
        bubble->setPosition({ origin.x + visibleSize.width * .6f, origin.y + visibleSize.height / 3 });
        bubble->setAnchorPoint({ .5f, 0 });
        bubble->setContentSize(bubble->getTextureRect().size);

        addChild(bubble);
    }

    auto text = ui::Text::create("", fontBasic, fontSizeMedium);
    {
        text->setAnchorPoint({ .5f, .5f });
        text->setTextColor(Color4B::BLACK);

        bubble->addChild(text);
    }

    // Begin Sequence
    lhs::Model::Painting const** selection = new lhs::Model::Painting const*;
    auto winnerMessage = new std::string;
    auto startMessage = new std::string;
    auto messages = std::vector<std::string*>{
        startMessage,
        new std::string{ lhs::Utility::ConvertToAscii(u8"10������\n�����մϴ�.") },
        new std::string{ lhs::Utility::ConvertToAscii(u8"����\n�����մϴ�!") },
    };

    auto getDataAction = CallFunc::create([=]()
        {
            *selection = lhs::Manager::SingleGameManager::Instance().GetSelectionForAuction();
            *startMessage = (*selection)->painter + lhs::Utility::ConvertToAscii(u8"\n�� ��ǰ�Դϴ�.");
            
            auto info = player->GetInformations();

            // �����͸� ������Ʈ�ϰ� ������
            // �̻��ϰ� padding �ɼ��� �� ����
            auto gallery = ui::ListView::create();
            {
                gallery->setBackGroundImage("PaintersInfoBoard.png");
                gallery->setContentSize(gallery->getBackGroundImageTextureSize());
                gallery->setAnchorPoint({ .5f, .1f });
                gallery->setPosition({ origin.x + visibleSize.width / 8, origin.y + visibleSize.height - gallery->getContentSize().height });
                gallery->setPadding(10, 10, 10, 10);

                auto title = ui::Text::create("My Gallery", fontBasic, fontSizeMedium);
                title->setTextColor({ 0x58, 0xcd, 0xf8, 0xFF });
                gallery->addChild(title);

                for (const auto& [painter, nPieces] : info)
                {
                    std::stringstream ss{};
                    ss << painter << ": " << nPieces;

                    auto text = ui::Text::create(ss.str(), fontBasic, fontSizeSmall);
                    gallery->addChild(text);
                }
                addChild(gallery, 0, 0x11112222);
            }

            auto artInfo = ui::ListView::create();
            {
                artInfo->setBackGroundImage("ArtInfoBoard.png");
                artInfo->setContentSize(artInfo->getBackGroundImageTextureSize());
                artInfo->setAnchorPoint({ .5f, .1f });
                artInfo->setPosition({ origin.x + visibleSize.width * 6 / 7, origin.y + visibleSize.height - artInfo->getContentSize().height });
                artInfo->setPadding(20, 20, 20, 20);
                artInfo->setItemsMargin(10);

                artInfo->addChild(ui::Text::create((*selection)->painter, fontBasic, fontSizeMedium));
                artInfo->addChild(ui::Text::create((*selection)->title, fontBasic, fontSizeSmall));
                addChild(artInfo, 0, 0x33334444);
            }
        });
    auto uiSetupAction = CallFunc::create([=]()
        {
            if (auto painting = ui::Painting::create(*selection))
            {
                painting->setScale(2.5f);
                painting->setAnchorPoint({ .5f, 0 });
                painting->setPosition({ easel->getPositionX() + 10, easel->getPositionY() + easel->getContentSize().height - painting->getBottomPadding() + 10 });

                addChild(painting, 0, 0x12345678);
            }
            bubble->setOpacity(0);
            bidButton->setEnabled(false);
        });
    auto beginSequence = Sequence::create(getDataAction, uiSetupAction, nullptr);

    // Playing Sequence
    lhs::Manager::SingleGameManager::Instance().Bid({ 1, cocos2d::RandomHelper::random_int<int>(0, 20) });
    lhs::Manager::SingleGameManager::Instance().Bid({ 2, cocos2d::RandomHelper::random_int<int>(0, 20) });
    lhs::Manager::SingleGameManager::Instance().Bid({ 3, cocos2d::RandomHelper::random_int<int>(0, 20) });

    auto delayAction = DelayTime::create(delay);

    auto createMessageAction = [&](const std::string* message) {
        auto showText = CallFunc::create([=]() {
            text->setString(*message);
            bubble->setOpacity(0xFF);
            bubble->setContentSize(text->getContentSize() + Size{ 20.f, 20.f });
            text->setPosition(bubble->getContentSize() / 2);
            });
        auto hideText = CallFunc::create([=]() {
            text->setString("");
            bubble->setOpacity(0);
            bubble->setContentSize(text->getContentSize() + Size{ 20.f, 20.f });
            text->setPosition(bubble->getContentSize() / 2);
            });
        auto showAction = Sequence::create(FadeIn::create(showDuration), nullptr);
        auto hideAction = Sequence::create(DelayTime::create(hideDuration), FadeOut::create(hideDuration), nullptr);

        return Sequence::create(showAction, showText, hideAction, hideText, nullptr);
    };

    // 1. ��� �Ұ�
    auto guideSequence = createMessageAction(messages.at(0));
    for (int i = 1; i < messages.size(); i++)
    {
        auto messageAction = createMessageAction(messages.at(i));
        guideSequence = Sequence::create(guideSequence, messageAction, nullptr);
    }

    // 2. ��� ����
    auto uiUpdateAction = CallFunc::create([=]()
        {
            bidButton->setEnabled(true);
            timer->start();
        });
    auto auctionPlayingSequence = Sequence::create(uiUpdateAction, DelayTime::create(32.f), nullptr);

    // 3. ��� ��ǥ
    auto timerResetAction = CallFunc::create([=]()
        {
            timer->reset(30);
        });
    for (auto n : { 1,2,3,4,5 })
    {
        if (auto board = Sprite::create("BidBoard.png"))
        {
            board->setPosition({ origin.x + visibleSize.width / 10 * (n * 2 - 1), origin.y + visibleSize.height / 6 });
            board->setAnchorPoint({ .5f, 0 });

            //addChild(board);
        }
    }
    auto showResultAction = CallFunc::create([=]()
        {
            auto [id, gold] = lhs::Manager::SingleGameManager::Instance().GetWinningBid();
            auto winner = lhs::Manager::PlayerManager::Instance().GetPlayer(id);

            lhs::u8stringstream ss{};
            ss << lhs::Utility::ConvertToUtf8(winner->GetName()) << u8"����" << std::endl;
            ss << lhs::Utility::ConvertToUtf8(std::to_string(gold)) << u8"��忡" << std::endl;
            ss << u8"�����߽��ϴ�!";

            *winnerMessage = lhs::Utility::ConvertToAscii(ss.str());
            winner->UpdateGold(-gold);

            std::stringstream sss{};
            sss << "Gold: " << player->GetGold();
            bidField->setPlaceHolder(sss.str());

            player->AddPainting(const_cast<lhs::Model::Painting*>(*selection));
        });

    auto showText = CallFunc::create([=]() {
        text->setString(*winnerMessage);
        bubble->setOpacity(0xFF);
        bubble->setContentSize(text->getContentSize() + Size{ 20.f, 20.f });
        text->setPosition(bubble->getContentSize() / 2);
        });
    auto hideText = CallFunc::create([=]() {
        text->setString("");
        bubble->setOpacity(0);
        bubble->setContentSize(text->getContentSize() + Size{ 20.f, 20.f });
        text->setPosition(bubble->getContentSize() / 2);
        });
    auto showAction = Sequence::create(FadeIn::create(showDuration), nullptr);
    auto hideAction = Sequence::create(DelayTime::create(hideDuration), FadeOut::create(hideDuration), nullptr);

    auto finishSequence = Sequence::create(showAction, showText, hideAction, hideText, nullptr);
    auto showResultSequence = Sequence::create(timerResetAction, showResultAction, finishSequence, nullptr);

    auto playingSequence = Sequence::create
    (
        guideSequence,
        auctionPlayingSequence,
        showResultSequence,
        nullptr
    );

    // End Sequence
    auto uiCleanupAction = CallFunc::create([=]()
        {
            removeChildByTag(0x12345678);

            // �ӽ�
            removeChildByTag(0x11112222);
            removeChildByTag(0x33334444);
        });
    auto endSequence = Sequence::create(uiCleanupAction, nullptr);

    auto moveSceneAction = CallFunc::create([=]()
        {
            delete selection;
            delete winnerMessage;
            delete startMessage;

            auto scene = RankingResult::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
        });

    auto auction = Sequence::create(beginSequence, playingSequence, endSequence, nullptr);
    auto round = Sequence::create(auction, auction, auction, auction, moveSceneAction, nullptr);

    runAction(round);

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
