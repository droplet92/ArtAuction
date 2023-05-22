#include "AuctionScene.h"
#include "ExplanationScene.h"
#include "RankingResultScene.h"

#include <algorithm>
#include <ranges>

#include <AudioEngine.h>
#include <ccRandom.h>
#include <ui/CocosGUI.h>

#include <EventCheckingAction.h>
#include <Utility.h>
#include <Manager/PlayerManager.h>
#include <Manager/SingleGameManager.h>
#include <Widget/Painting.h>
#include <Widget/Popup.h>
#include <Widget/Timer.h>

USING_NS_CC;


constexpr auto fontBasic = "fonts/Dovemayo_gothic.ttf";
constexpr auto fontSizeMedium = 24;
constexpr auto fontSizeSmall = 16;

constexpr float delay = 2.f;
constexpr float showDuration = 1.f;
constexpr float hideDuration = 1.f;

constexpr float playtime = 30.f;

bool isBidValid(int bid, int lower, int upper)
{
    if (bid < lower) return false;
    if (bid > upper) return false;
    return true;
}

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
    ss << "front" << n << ".png";

    if (auto character = Sprite::createWithSpriteFrameName(ss.str()))
    {
        character->setPosition({ origin.x + visibleWidth / 8 * (n * 2 - 1), origin.y });
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
        return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add your codes below...
    auto temp = new std::vector<Action*>;
    auto bids = new std::vector<std::pair<int, int>>;

    auto player = lhs::Manager::PlayerManager::Instance().GetPlayer(0);
    auto [roundCount, roundType] = lhs::Manager::SingleGameManager::Instance().GetCurrentRound();
    auto minBid = new int;

    if ((roundType == u8"공개 입찰") || (roundType == u8"NFT 공개 입찰"))
        lhs::Manager::SingleGameManager::Instance().AddNewBidEventListener([=](int newBid) { *minBid = newBid; });

    if (auto background = Sprite::create("backgrounds/AuctionBackground.jpg"))
    {
        background->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 });
        background->setAnchorPoint({ .5f, .5f });

        addChild(background);
    }
    auto easel = Sprite::createWithSpriteFrameName("easel.png");
    {
        easel->setPosition({ origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 6 });
        easel->setAnchorPoint({ .5f, 0 });

        addChild(easel);
    }
    if (auto dealer = Sprite::createWithSpriteFrameName("dealer.png"))
    {
        dealer->setPosition({ origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 });
        dealer->setAnchorPoint({ .5f, .5f });

        addChild(dealer);
    }
    auto players = lhs::Manager::PlayerManager::Instance().GetRoomPlayers(0);
    std::vector<Sprite*> characters = {
        MakeCharacter(1, origin, visibleSize.width),
        MakeCharacter(2, origin, visibleSize.width),
        MakeCharacter(3, origin, visibleSize.width),
        MakeCharacter(4, origin, visibleSize.width),
        //MakeCharacter(5, origin, visibleSize.width),
    };
    for (const auto& character : characters)
        addChild(character);

    if (auto reputation = ui::ListView::create())
    {
        reputation->setBackGroundImage("PaintersInfoBoard.png", ui::Widget::TextureResType::PLIST);
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
    auto popup = ui::Popup::create({ .61f, .4f }, false);
    {
        popup->setVisible(false);
        addChild(popup, 1);
    }
    auto bidBoard = ui::Layout::create();
    {
        bidBoard->setBackGroundImage("popup.png", ui::Widget::TextureResType::PLIST);
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
    auto bidButton = ui::Button::create("PopupOk.png", "", "", ui::Widget::TextureResType::PLIST);
    {
        bidButton->setTitleFontName(fontBasic);
        bidButton->setTitleFontSize(fontSizeMedium);
        bidButton->setTitleText("Bid");
        bidButton->addTouchEventListener([=](auto _, ui::Widget::TouchEventType type)
            {
                if (type == ui::Widget::TouchEventType::BEGAN)
                {
                    auto alert = ui::Text::create();
                    {
                        alert->setFontName("fonts/Dovemayo_gothic.ttf");
                        alert->setFontSize(24);
                        alert->setTextColor(Color4B::BLACK);
                    }
                    try
                    {
                        auto offer = std::stoi(bidField->getString());
                        cocos2d::log("%d", offer);

                        if (!isBidValid(offer, *minBid, player->GetGold()))
                        {
                            if (popup->isVisible())
                                return;

                            alert->setString("Check your offer.");
                            popup->addContent(alert);
                            popup->setVisible(true);
                            return;
                        }
                        AudioEngine::play2d("audios/click.mp3");
                        lhs::Manager::SingleGameManager::Instance().Bid({ 0, offer });
                        bidField->setPlaceHolder("Offer: " + std::to_string(offer));
                    }
                    catch (const std::invalid_argument& e)
                    {
                        cocos2d::log("invalid %s: ", e.what());
                        alert->setString("Not a number.");
                        popup->addContent(alert);
                        popup->setVisible(true);
                    }
                    catch (const std::out_of_range& e)
                    {
                        cocos2d::log("OOR %s: ", e.what());
                        alert->setString("Not enough golds.");
                        popup->addContent(alert);
                        popup->setVisible(true);
                    }
                    catch (const std::exception& e)
                    {
                        cocos2d::log("%s: ", e.what());
                    }
                    bidField->setString("");
                }
            });
        bidButton->setAnchorPoint({ .1f, .5f });
        bidButton->setPosition({ bidBoard->getContentSize().width - bidButton->getContentSize().width,
                                    bidBoard->getContentSize().height / 2 });

        bidBoard->addChild(bidButton);
    }
    auto timer = ui::Timer::create(playtime);
    {
        timer->setPosition({ visibleSize.width * .5f, visibleSize.height * .85f });
        timer->setAlarm([]() {});

        addChild(timer);
    }
    auto bubble = ui::Scale9Sprite::createWithSpriteFrameName("bubble.png");
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
    auto bidMessage = new std::string;
    auto messages = std::vector<std::string*>{
        startMessage,
        bidMessage,
        new std::string{ lhs::Utility::ConvertToAscii(u8"시작합니다!") },
    };
    auto getDataAction = CallFunc::create([=]()
        {
            *selection = lhs::Manager::SingleGameManager::Instance().GetSelectionForAuction();
            *startMessage = (*selection)->painter + lhs::Utility::ConvertToAscii(u8"\n의 작품입니다.");
            *minBid = (roundType == u8"정찰") ? 10 : 1;
            *bidMessage = std::to_string(*minBid) + lhs::Utility::ConvertToAscii(u8" 골드 이상\n제시해주세요.");

            auto info = player->GetInformations();

            // 데이터만 업데이트하고 싶은데
            // 이상하게 padding 옵션이 안 먹음
            auto gallery = ui::ListView::create();
            {
                gallery->setBackGroundImage("PaintersInfoBoard.png", ui::Widget::TextureResType::PLIST);
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
                artInfo->setBackGroundImage("ArtInfoBoard.png", ui::Widget::TextureResType::PLIST);
                artInfo->setContentSize(artInfo->getBackGroundImageTextureSize());
                artInfo->setAnchorPoint({ .5f, .1f });
                artInfo->setPosition({ origin.x + visibleSize.width * 6 / 7, origin.y + visibleSize.height - artInfo->getContentSize().height });
                artInfo->setPadding(20, 20, 20, 20);
                artInfo->setItemsMargin(10);

                auto title = ui::Text::create((*selection)->title, fontBasic, fontSizeSmall);
                title->setTextAreaSize({ artInfo->getContentSize().width - 20, 100 });

                artInfo->addChild(ui::Text::create((*selection)->painter, fontBasic, fontSizeMedium));
                artInfo->addChild(title);
                addChild(artInfo, 0, 0x33334444);
            }
        });
    auto uiSetupAction = CallFunc::create([=]()
        {
            if (auto painting = ui::Painting::create(*selection))
            {
                painting->setScale(2.f);
                painting->setAnchorPoint({ .5f, 0 });
                painting->setPosition({ easel->getPositionX(),
                                        easel->getPositionY() + easel->getContentSize().height - painting->getBottomPadding() });

                addChild(painting, 0, 0x12345678);
            }
            for (int i = 0; i < characters.size(); i++)
            {
                characters[i]->initWithSpriteFrameName("front" + std::to_string(i + 1) + ".png");
                characters[i]->setAnchorPoint({ .5f, 0 });
            }
            bubble->setOpacity(0);
            bidButton->setEnabled(false);
        });
    auto beginSequence = Sequence::create(getDataAction, uiSetupAction, nullptr);

    // Playing Sequence
    auto delayAction = DelayTime::create(delay);
    auto createMessageAction = [&](const std::string* message)
    {
        auto showText = CallFunc::create([=]()
            {
                text->setString(*message);
                bubble->setOpacity(0xFF);
                bubble->setContentSize(text->getContentSize() + Size{ 20.f, 20.f });
                text->setPosition(bubble->getContentSize() / 2);
            });
        auto hideText = CallFunc::create([=]()
            {
                text->setString("");
                bubble->setOpacity(0);
                bubble->setContentSize(text->getContentSize() + Size{ 20.f, 20.f });
                text->setPosition(bubble->getContentSize() / 2);
            });
        auto showAction = Sequence::create(FadeIn::create(showDuration), nullptr);
        auto hideAction = Sequence::create(DelayTime::create(hideDuration), FadeOut::create(hideDuration), nullptr);

        return Sequence::create(showAction, showText, hideAction, hideText, nullptr);
    };

    // 1. 경매 소개
    auto guideSequence = createMessageAction(messages.at(0));
    for (int i = 1; i < messages.size(); i++)
    {
        auto messageAction = createMessageAction(messages.at(i));
        guideSequence = Sequence::create(guideSequence, messageAction, nullptr);
    }

    // 2. 경매 진행
    auto uiUpdateAction = CallFunc::create([=]()
        {
            bidButton->setEnabled(true);
            timer->start();
        });
    auto biddingAction = CallFunc::create([=]()
        {
            if (roundType == u8"공개 입찰")
            {
                schedule([=](float t)
                    {
                        cocos2d::log("open1 3");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 1, *minBid + 1 });
                    }, 3.f, 9, 0, "open1");
                schedule([=](float t)
                    {
                        cocos2d::log("open2 5 +1");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 2, *minBid + 1 });
                    }, 5.f, 3, 1, "open2");
                schedule([=](float t)
                    {
                        cocos2d::log("open3 7");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 3, *minBid + 1 });
                    }, 7.f, 2, 0, "open3");
            }
            else if (roundType == u8"비공개 입찰")
            {
                scheduleOnce([=](float t)
                    {
                        cocos2d::log("closed1 3");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 1, cocos2d::RandomHelper::random_int<int>(*minBid, std::min(20, int(players[1]->GetGold()))) });
                    }, 3, "closed1");
                scheduleOnce([=](float t)
                    {
                        cocos2d::log("closed2 10");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 2, cocos2d::RandomHelper::random_int<int>(*minBid, std::min(20, int(players[2]->GetGold()))) });
                    }, 10, "closed2");
                scheduleOnce([=](float t)
                    {
                        cocos2d::log("closed3 14");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 3, cocos2d::RandomHelper::random_int<int>(*minBid, std::min(20, int(players[3]->GetGold()))) });
                    }, 14, "closed3");
            }
            else if (roundType == u8"정찰")
            {
                schedule([=](float t)
                    {
                        cocos2d::log("fixed1 5 +1");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 1, *minBid });
                    }, 5, 3, 1, "fixed1");
                scheduleOnce([=](float t)
                    {
                        cocos2d::log("fixed2 10");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 2, *minBid });
                    }, 10, "fixed2");
                scheduleOnce([=](float t)
                    {
                        cocos2d::log("fixed3 15");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 3, *minBid });
                    }, 15, "fixed3");
            }
            else if (roundType == u8"NFT 공개 입찰")
            {
                schedule([=](float t)
                    {
                        cocos2d::log("nft1 3");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 1, *minBid + 1 });
                    }, 3.f, 9, 0, "nft1");
                schedule([=](float t)
                    {
                        cocos2d::log("nft2 5 +1");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 2, *minBid + 1 });
                    }, 5.f, 3, 1, "nft2");
                schedule([=](float t)
                    {
                        cocos2d::log("nft3 7");
                        if (cocos2d::RandomHelper::random_int<int>(0, 1))
                            lhs::Manager::SingleGameManager::Instance().Bid({ 3, *minBid + 1 });
                    }, 7.f, 2, 0, "nft3");
            }
        });
    auto timerWaitingAction = EventCheckingAction::create(playtime + delay, [=]()
        {
            stopAllActions();

            auto round = Sequence::create(CallFunc::create({}), nullptr);

            for (int i = 6; i < temp->size(); i++)
                round = Sequence::create(round, temp->at(i), nullptr);

            // 원인불명
            if (roundType == u8"정찰")
            {
                if (lhs::Manager::SingleGameManager::Instance().IsRoundEnd())
                {
                    auto moveSceneAction = CallFunc::create([=]()
                        {
                            delete minBid;
                            delete selection;
                            delete winnerMessage;
                            delete startMessage;
                            delete bidMessage;
                            delete bids;

                            auto scene = (roundCount < 4)
                                ? Explanation::createScene()
                                : RankingResult::createScene();
                            Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
                        });
                    round = Sequence::create(round, moveSceneAction, nullptr);
                }
                else
                {
                    for (auto elem : *temp)
                        round = Sequence::create(round, elem, nullptr);
                }
            }

            runAction(round);
        });
    auto afterBidAction = CallFunc::create([=]()
        {
            bidButton->setEnabled(false);
            timer->reset(playtime);
            unscheduleAllCallbacks();
        });
    auto auctionPlayingSequence = Sequence::create(uiUpdateAction, timerWaitingAction, nullptr);

    // 3. 결과 발표
    auto dummyAction = CallFunc::create({});
    auto showBidSequence = Sequence::create(dummyAction, nullptr);
    auto showAction = Sequence::create(FadeIn::create(showDuration), nullptr);
    auto hideAction = Sequence::create(DelayTime::create(hideDuration), FadeOut::create(hideDuration), nullptr);

    for (int i = 0; i < characters.size(); i++)
    {
        auto showBidAction = CallFunc::create([=]()
            {
                characters[i]->initWithSpriteFrameName("back" + std::to_string(i + 1) + ".png");
                characters[i]->setAnchorPoint({ .5f, 0 });
            });
        showBidSequence = Sequence::create(showBidSequence, showAction, showBidAction, nullptr);
    }
    for (int i = 0; i < characters.size(); i++)
    {
        auto showBidAction = CallFunc::create([=]()
            {
                auto bidder = players[i];
                auto iter = std::ranges::find_if(*bids, [=](auto bid) { return bid.first == bidder->GetId(); });
                auto bid = (iter != std::end(*bids)) ? iter->second : 0;

                if (auto board = Sprite::createWithSpriteFrameName("BidBoard.png"))
                {
                    board->setPosition(characters[i]->getPosition() + Vec2{ 0, 150 });
                    board->setAnchorPoint({ .5f, 0 });

                    auto text = ui::Text::create(std::to_string(bid), fontBasic, fontSizeMedium);
                    text->setTextColor(Color4B::BLACK);
                    text->setPosition(board->getContentSize() / 2);
                    text->setAnchorPoint({ .5f, .5f });

                    board->addChild(text);

                    addChild(board, 0, bidder->GetId());
                }
                characters[i]->initWithSpriteFrameName("bid" + std::to_string(i + 1) + ".png");
                characters[i]->setAnchorPoint({ .5f, 0 });
            });
        showBidSequence = Sequence::create(showBidSequence, showAction, showBidAction, nullptr);
    }
    auto dataUpdateAction = CallFunc::create([=]()
        {
            *bids = lhs::Manager::SingleGameManager::Instance().GetBids();
            timer->reset(playtime);
        });
    auto showResultAction = CallFunc::create([=]()
        {
            auto [id, gold] = lhs::Manager::SingleGameManager::Instance().GetWinningBid();
            auto winner = lhs::Manager::PlayerManager::Instance().GetPlayer(id);

            lhs::u8stringstream ss{};
            ss << lhs::Utility::ConvertToUtf8(winner->GetName()) << u8" 님이" << std::endl;
            ss << lhs::Utility::ConvertToUtf8(std::to_string(gold)) << u8"골드에" << std::endl;
            ss << u8"낙찰했습니다!";

            *winnerMessage = lhs::Utility::ConvertToAscii(ss.str());
            winner->UpdateGold(-gold);

            std::stringstream sss{};
            sss << "Gold: " << player->GetGold();
            bidField->setPlaceHolder(sss.str());

            winner->AddPainting(const_cast<lhs::Model::Painting*>(*selection));
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

    auto finishSequence = Sequence::create(showAction, showText, hideAction, hideText, nullptr);
    auto showResultSequence = Sequence::create(dataUpdateAction, showBidSequence, showResultAction, finishSequence, nullptr);

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
            if (roundType != u8"정찰")
            {
                stopAllActions();
                auto round = Sequence::create(CallFunc::create({}), nullptr);

                if (lhs::Manager::SingleGameManager::Instance().IsRoundEnd())
                {
                    auto moveSceneAction = CallFunc::create([=]()
                        {
                            delete minBid;
                            delete selection;
                            delete winnerMessage;
                            delete startMessage;
                            delete bidMessage;
                            delete bids;

                            auto scene = (roundCount < 4)
                                ? Explanation::createScene()
                                : RankingResult::createScene();
                            Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
                        });
                    round = Sequence::create(round, moveSceneAction, nullptr);
                }
                else
                {
                    for (auto elem : *temp)
                        round = Sequence::create(round, elem, nullptr);
                }
                runAction(round);
            }

            removeChildByTag(0x12345678);

            // 임시
            removeChildByTag(0x11112222);
            removeChildByTag(0x33334444);

            for (const auto& player : players)
                removeChildByTag(player->GetId());
        });
    auto endSequence = Sequence::create(uiCleanupAction, nullptr);
    temp->assign({
        // begin sequence
        getDataAction,
        uiSetupAction,
        // playing sequence
        guideSequence,
        uiUpdateAction,
        biddingAction,
        timerWaitingAction,
        afterBidAction,
        showResultSequence,
        // end sequence
        uiCleanupAction,
        nullptr
    });
    auto round = Sequence::create(CallFunc::create({}), nullptr);

    for (auto elem : *temp)
        round = Sequence::create(round, elem, nullptr);

    runAction(round);

    return true;
}

void Auction::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
