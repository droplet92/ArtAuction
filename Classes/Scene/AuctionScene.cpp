#include "AuctionScene.h"
#include "ReputationScene.h"

#include <algorithm>
#include <format>
#include <ranges>

#include <ccRandom.h>
#include <ui/CocosGUI.h>
USING_NS_CC;
using namespace ui;

#include <EventCheckingAction.h>
#include <Utility.h>
using namespace lhs;

#include <Manager/PlayerManager.h>
#include <Manager/SingleGameManager.h>
using namespace lhs::manager;
using namespace lhs::model;

#include <Widget/BidBoard.h>
#include <Widget/Character.h>
#include <Widget/InformationBoard.h>
#include <Widget/Painting.h>
#include <Widget/SpeechBubble.h>
#include <Widget/Timer.h>
using namespace lhs::widget;


constexpr float delay = 2.f;
constexpr float playtime = 30.f;

static std::vector<Character*> GetCharacters(const std::vector<Player*>& players)
{
    std::vector<Character*> characters{};

    for (size_t i = 0; i < players.size(); i++)
    {
        std::stringstream ss{};
        ss << "front" << i + 1 << ".png";

        auto character = Utility::Create<Character>();
        character->SetId(i + 1);
        character->Attention();
        character->SetName(players[i]->GetName());

        characters.push_back(character);
    }
    return characters;
}

void ChangeScene()
{
    auto scene = Reputation::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
}

Auction::Auction()
    : minBid(0)
{
}

Scene* Auction::createScene()
{
    return Auction::create();
}

bool Auction::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    const auto origin = Director::getInstance()->getVisibleOrigin();
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    const auto [roundCount, roundType] = SingleGameManager::Instance().GetCurrentRound();
    const auto players = PlayerManager::Instance().GetRoomPlayers(0);
    const auto player = PlayerManager::Instance().GetPlayer(0);
    const auto reputationData = SingleGameManager::Instance().GetReputation();

    // 2. Create and initialize items
    //
    auto background = Utility::CreateWithFile<Sprite>("backgrounds/AuctionBackground.jpg");
    background->setPosition(origin + visibleSize / 2);
    background->setAnchorPoint({ .5f, .5f });

    auto easel = Utility::CreateWithSprite<Sprite>("easel.png");
    easel->setPosition(origin + Vec2{ visibleSize.width / 3, visibleSize.height / 6 });
    easel->setAnchorPoint({ .5f, 0 });

    auto galleryBoard = Utility::CreateWithSprite<InformationBoard>("PaintersInfoBoard.png");
    galleryBoard->setPosition({ origin.x + visibleSize.width / 8, origin.y + visibleSize.height - galleryBoard->getContentSize().height });
    galleryBoard->SetTitle("My Gallery");

    auto reputation = Utility::CreateWithSprite<InformationBoard>("PaintersInfoBoard.png");
    reputation->setPosition(origin + Vec2{ visibleSize.width * 3 / 10, visibleSize.height - reputation->getContentSize().height });
    reputation->SetTitle("Reputation");
    reputation->SetInformation(reputationData);

    auto artInfo = Utility::CreateWithSprite<InformationBoard>("ArtInfoBoard.png");
    artInfo->setPosition({ origin.x + visibleSize.width * 6 / 7, origin.y + visibleSize.height - artInfo->getContentSize().height });

    auto timer = Utility::Create<widget::Timer>();
    timer->setPosition({ visibleSize.width * .5f, visibleSize.height * .85f });
    timer->SetRemainTime(playtime);

    auto popup = Utility::Create<Popup>();
    popup->SetScale({ .61f, .4f });
    popup->setVisible(false);

    auto bidBoard = Utility::Create<BidBoard>();
    bidBoard->setPosition({ visibleSize.width * 6 / 7, visibleSize.height / 2 });
    bidBoard->RegisterPopup(popup);
    bidBoard->RegisterBidder(player);
    bidBoard->RegisterMinimumBid(minBid);
    bidBoard->UpdateGold();

    std::vector<Character*> characters = GetCharacters(players);

    for (size_t i = 0; i < characters.size(); i++)
        characters[i]->setPosition(origin + Vec2{ visibleSize.width / 8 * ((i + 1) * 2 - 1), 0.f });

    auto dealer = Utility::Create<Character>();
    dealer->SetImage("dealer.png");
    dealer->setPosition(origin + Vec2{ visibleSize.width / 2, visibleSize.height / 4 });

    auto painting = Utility::Create<widget::Painting>();
    painting->setAnchorPoint({ .5f, 0 });

    // 3. Add items to scene
    //
    addChild(background, -1);
    addChild(easel);
    addChild(painting);
    addChild(galleryBoard);
    addChild(reputation);
    addChild(artInfo);
    addChild(timer);
    addChild(popup, 1);
    addChild(bidBoard);
    addChild(dealer);

    for (size_t i = 0; i < characters.size(); i++)
        addChild(characters[i]);

    //if ((roundType == u8"공개 입찰") || (roundType == u8"NFT 공개 입찰"))
    if ((roundType == u8"Open Bidding") || (roundType == u8"NFT Open Bidding") || (roundType == u8"Fixed Price"))
    {
        SingleGameManager::Instance().AddNewBidEventListener([=](const std::pair<int, int>& newBid)
            {
                const auto& [bidderId, bid] = newBid;
                auto bidder = characters[bidderId];

                minBid = bid + 1;
                bidder->ShowBid(bid);
            });
    }

    // Begin Sequence
    //
    auto dummyAction = CallFunc::create({});
    auto delayAction = DelayTime::create(1.f);

    auto messages = std::vector<std::string*>
    {
        &startMessage,
        &bidMessage,
        new std::string{ Utility::ConvertToAscii(u8"We start\nthe bidding!") },
        //new std::string{ Utility::ConvertToAscii(u8"시작합니다!") },
    };
    auto getDataAction = CallFunc::create([=]()
        {
            selection = SingleGameManager::Instance().GetSelectionForAuction();
            startMessage = std::format("This is by\n{}", selection.painter);
            //*startMessage = (*selection)->painter + Utility::ConvertToAscii(u8"\n의 작품입니다.");
            minBid = (roundType == u8"Fixed Price") ? 10 : 1;
            bidMessage = std::format("Please bid\nat least {}", minBid);
            //*bidMessage = std::to_string(*minBid) + Utility::ConvertToAscii(u8" 골드 이상\n제시해주세요.");

            const auto data = player->GetInformations();
            galleryBoard->SetInformation(data);

            artInfo->SetTitle(selection.painter);
            artInfo->SetInformation(selection.title);
        });
    auto uiSetupAction = CallFunc::create([=]()
        {
            painting->SetImage(selection);
            painting->setScale(2.f);
            painting->setPosition(easel->getPosition() + Vec2{ 0, easel->getContentSize().height - painting->GetBottomPadding() });

            for (size_t i = 0; i < characters.size(); i++)
                characters[i]->Attention();

            bidBoard->setEnabled(false);
        });

    // Playing Sequence
    //
    auto createMessageAction = [&](const std::string* message)
    {
        auto showText = CallFunc::create([=]() { dealer->Say(*message); });

        return Sequence::create(delayAction, showText, delayAction, nullptr);
    };

    // 1. 경매 소개
    auto guideSequence = createMessageAction(messages.at(0));
    for (size_t i = 1; i < messages.size(); i++)
    {
        auto messageAction = createMessageAction(messages.at(i));
        guideSequence = Sequence::create(guideSequence, messageAction, nullptr);
    }

    // 2. 경매 진행
    auto uiUpdateAction = CallFunc::create([=]()
        {
            bidBoard->setEnabled(true);
            timer->Start();
        });
    auto biddingAction = CallFunc::create([=]()
        {
            auto joinOpenBid = [=](int playerId)
            {
                if (players[playerId]->GetGold() <= minBid) return;
                if (!RandomHelper::random_int(0, 1)) return;
                SingleGameManager::Instance().Bid({ playerId, minBid });
            };
            auto joinClosedBid = [=](int playerId)
            {
                auto bid = std::min(20, int(players[playerId]->GetGold()));
                if (bid < minBid) return;
                if (!RandomHelper::random_int(0, 1)) return;
                SingleGameManager::Instance().Bid({ playerId, RandomHelper::random_int(minBid, bid) });
            };
            auto joinFixedPrice = [=](int playerId)
            {
                if (players[playerId]->GetGold() <= minBid) return;
                if (!RandomHelper::random_int(0, 1)) return;
                SingleGameManager::Instance().Bid({ playerId, minBid });
            };
            if (roundType == u8"Open Bidding")
            {
                schedule([=](float t) { joinOpenBid(1); }, 3.f, 9, 0, "open1");
                schedule([=](float t) { joinOpenBid(2); }, 5.f, 3, 1, "open2");
                schedule([=](float t) { joinOpenBid(3); }, 7.f, 2, 0, "open3");
            }
            else if (roundType == u8"Closed Bidding")
            {
                scheduleOnce([=](float t) { joinClosedBid(1); }, 3, "closed1");
                scheduleOnce([=](float t) { joinClosedBid(2); }, 10, "closed2");
                scheduleOnce([=](float t) { joinClosedBid(3); }, 14, "closed3");
            }
            else if (roundType == u8"Fixed Price")
            {
                schedule([=](float t) { joinFixedPrice(1); }, 5, 3, 1, "fixed1");
                scheduleOnce([=](float t) { joinFixedPrice(2); }, 10, "fixed2");
                scheduleOnce([=](float t) { joinFixedPrice(3); }, 15, "fixed3");
            }
            else if (roundType == u8"NFT Open Bidding")
            {
                schedule([=](float t) { joinOpenBid(1); }, 3.f, 9, 0, "nft1");
                schedule([=](float t) { joinOpenBid(2); }, 5.f, 3, 1, "nft2");
                schedule([=](float t) { joinOpenBid(3); }, 7.f, 2, 0, "nft3");
            }
        });
    auto timerWaitingAction = BidCompleteCheckingAction::create(playtime + delay, [=]()
        {
            stopAllActions();

            auto round = Sequence::create(dummyAction, nullptr);

            for (size_t i = 6; i < gamePlayActions.size(); i++)
                round = Sequence::create(round, gamePlayActions.at(i), nullptr);

            // 원인불명
            if (roundType == u8"Fixed Price")
            {
                if (SingleGameManager::Instance().IsRoundEnd())
                {
                    auto changeSceneAction = CallFunc::create(ChangeScene);
                    round = Sequence::create(round, changeSceneAction, nullptr);
                }
                else
                {
                    for (auto action : gamePlayActions)
                        round = Sequence::create(round, action, nullptr);
                }
            }

            runAction(round);
        });
    auto afterBidAction = CallFunc::create([=]()
        {
            bidBoard->setEnabled(false);
            timer->Reset(playtime);
            unscheduleAllCallbacks();
        });

    // 3. 결과 발표
    //
    auto showBidSequence = Sequence::create(dummyAction, nullptr);
    auto showAction = Sequence::create(FadeIn::create(1.f), nullptr);
    auto hideAction = Sequence::create(DelayTime::create(5.f), nullptr);

    auto dataUpdateAction = CallFunc::create([=]()
        {
            bids = SingleGameManager::Instance().GetBids();
            timer->Reset(playtime);
        });
    if (roundType == u8"Closed Bidding")
    {
        for (size_t i = 0; i < characters.size(); i++)
        {
            auto showBidAction = CallFunc::create([=]()
                {
                    auto iter = std::ranges::find_if(bids, [=](auto bid) { return bid.first == players[i]->GetId(); });
                    auto bid = (iter != std::end(bids)) ? iter->second : 0;

                    characters[i]->OpenBid(bid);
                });
            showBidSequence = Sequence::create(showBidSequence, showBidAction, nullptr);
        }
        showBidSequence = Sequence::create(showBidSequence, DelayTime::create(7.f), nullptr);
    }
    auto updateResultAction = CallFunc::create([=]()
        {
            auto [id, gold] = SingleGameManager::Instance().GetWinningBid();
            auto winner = PlayerManager::Instance().GetPlayer(id);

            // Update Result
            //
            //ss << Utility::ConvertToUtf8(winner->GetName()) << u8" 님이" << std::endl;
            //ss << Utility::ConvertToUtf8(std::to_string(gold)) << u8"골드에" << std::endl;
            //ss << u8"낙찰됐습니다!";
            cocos2d::log("%s winner: [id %u] bid %u", Utility::ConvertToAscii(roundType).c_str(), winner->GetId(), gold);
            winnerMessage = std::format("{} has\nwon your bid\nfor {} gold!", winner->GetName(), gold);
            winner->AddGold(-gold);

            if (roundType == u8"NFT Open Bidding")
                winner->AddGold(SingleGameManager::Instance().GetRealTimeNftPrice());
            else
                winner->AddPainting(selection);

            SingleGameManager::Instance().AddWinningBid(selection.painter, gold);
            bidBoard->UpdateGold();
        });
    auto showText = CallFunc::create([=]() { dealer->Say(winnerMessage); });
    auto finishSequence = Sequence::create(delayAction, showText, delayAction, nullptr);
    auto updateResultSequence = Sequence::create(dataUpdateAction, showBidSequence, updateResultAction, finishSequence, nullptr);

    // End Sequence
    auto uiCleanupAction = CallFunc::create([=]()
        {
            if (roundType != u8"Fixed Price")
            {
                stopAllActions();
                auto round = Sequence::create(dummyAction, nullptr);

                if (SingleGameManager::Instance().IsRoundEnd())
                {
                    auto changeSceneAction = CallFunc::create(ChangeScene);
                    round = Sequence::create(round, changeSceneAction, nullptr);
                }
                else
                {
                    for (auto action : gamePlayActions)
                        round = Sequence::create(round, action, nullptr);
                }
                runAction(round);
            }

            for (auto& character : characters)
                character->Attention();
        });
    gamePlayActions.assign({
        // begin sequence
        getDataAction,
        uiSetupAction,
        // playing sequence
        guideSequence,
        uiUpdateAction,
        biddingAction,
        timerWaitingAction,
        afterBidAction,
        updateResultSequence,
        // end sequence
        uiCleanupAction,
        nullptr
    });
    auto round = Sequence::create(dummyAction, nullptr);

    for (auto action : gamePlayActions)
        round = Sequence::create(round, action, nullptr);

    runAction(round);

    return true;
}
