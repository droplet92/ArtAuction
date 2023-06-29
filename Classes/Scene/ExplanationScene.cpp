#include "ExplanationScene.h"
#include "MyGalleryScene.h"

#include <string>
#include <unordered_map>

#include <AudioEngine.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>
#include <ScriptSequenceGenerator.h>
#include <Manager/SingleGameManager.h>
using namespace lhs;
using namespace lhs::manager;


const std::unordered_map<std::u8string, std::vector<std::u8string>> scriptMap =
{
    { u8"Open Bidding",
        {
            u8"Each player publicly offers a price.",
            u8"When the time expires, the last player with the highest bid wins.",
            u8"The purchased artwork is added to their gallery.",
        }
    },
    { u8"Closed Bidding",
        {
            u8"Each player privately offers a price.",
            u8"When the time expires, the first player with the highest bid wins.",
            u8"The purchased artwork is added to their gallery.",
        }
    },
    { u8"Fixed Price",
        {
            u8"Each player publicly offers a price.",
            u8"The first player to place a bid wins.",
            u8"The purchased artwork is added to their gallery.",
        }
    },
    { u8"NFT Open Bidding",
        {
            u8"Each player publicly offers a price.",
            u8"When the time expires, the player with the highest bid wins.",
            u8"The purchased artwork is immediately sold at market price (randomized).",
        }
    },
    //{ u8"공개 입찰",
    //    {
    //        u8"각 플레이어는 공개로 가격을 제시합니다.",
    //        u8"시간이 만료되면 가장 마지막에 최고액을 작성했던 플레이어가 낙찰합니다.",
    //        u8"구매한 작품은 자신의 갤러리에 추가됩니다.",
    //    }
    //},
    //{ u8"비공개 입찰",
    //    {
    //        u8"각 플레이어는 비공개로 가격을 제시합니다.",
    //        u8"시간이 만료되면 가장 먼저 최고액을 작성했던 플레이어가 낙찰합니다.",
    //        u8"구매한 작품은 자신의 갤러리에 추가됩니다.",
    //    }
    //},
    //{ u8"정찰",
    //    {
    //        u8"각 플레이어는 공개로 가격을 제시합니다.",
    //        u8"가장 먼저 제시 금액을 작성했던 플레이어가 낙찰합니다.",
    //        u8"구매한 작품은 자신의 갤러리에 추가됩니다.",
    //    }
    //},
    //{ u8"NFT 공개 입찰",
    //    {
    //        u8"각 플레이어는 공개로 가격을 제시합니다.",
    //        u8"시간이 만료되면 가장 마지막에 최고액을 작성했던 플레이어가 낙찰합니다.",
    //        u8"구매한 작품은 즉시 시세가(랜덤)에 판매됩니다.",
    //    }
    //},
};

static void ChangeScene()
{
    auto scene = MyGallery::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
};

std::vector<std::u8string> GetScript(const std::u8string& roundType)
{
    const auto& lines = scriptMap.at(roundType);
    return
    {
        u8"This round is a(n) " + roundType + u8" trade.",
            u8"It goes on 30 seconds per artwork.",
            lines.at(0),
            lines.at(1),
            lines.at(2),
            u8"The artist's reputation increases in order of the highest bidder."
            //u8"이번 라운드는 " + roundType + u8" 거래입니다.",
            //u8"작품 하나당 30초간 진행됩니다.",
            //lines.at(0),
            //lines.at(1),
            //lines.at(2),
            //u8"가장 높은 금액이 거래된 작가 순으로 작가의 평판이 상승합니다."
    };
}

Scene* Explanation::createScene()
{
    return Explanation::create();
}

bool Explanation::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    SingleGameManager::Instance().MoveToNextRound();
    const auto [nextRound, roundType] = SingleGameManager::Instance().GetCurrentRound();
    const auto roundString = "Round " + std::to_string(nextRound);
    const auto script = GetScript(roundType);

    // 2. Create and initialize items
    //
    setColor(Color3B::BLACK);

    auto title = Utility::CreateWithFile<Label>(roundString, FONT_SIZE_LARGE);
    title->setPosition(origin + Vec2{ visibleSize.width / 2, visibleSize.height - title->getContentSize().height });

    auto scriptSequence = ScriptSequenceGenerator::Generate(script, [=](Label* line) { addChild(line); });
    auto changeSceneAction = CallFunc::create(ChangeScene);

    // 3. Add items to scene
    //
    addChild(title);
    runAction(Sequence::create(scriptSequence, changeSceneAction, nullptr));

    return true;
}
