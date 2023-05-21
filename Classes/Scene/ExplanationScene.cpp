#include "ExplanationScene.h"
#include "MyGalleryScene.h"

#include <unordered_map>
#include <sstream>
#include <string>

#include <Manager/SingleGameManager.h>
#include <Manager/PlayerManager.h>
#include "../Utility.h"
#include <audio/include/AudioEngine.h>

USING_NS_CC;


constexpr float delay = 1.0f;
constexpr float fadeInDuration = 0.5f;

const std::unordered_map<std::u8string, std::vector<std::u8string>> scriptMap =
{
    { u8"비공개",
        {
            u8"각 플레이어는 비공개로 가격을 제시합니다.",
            u8"시간이 만료되면 가장 먼저 최고액을 작성했던 플레이어가 낙찰합니다.",
            u8"구매한 작품은 자신의 갤러리에 추가됩니다.",
        }
    },
    { u8"정찰제",
        {
            u8"각 플레이어는 공개로 가격을 제시합니다.",
            u8"가장 먼저 제시 금액을 작성했던 플레이어가 낙찰합니다.",
            u8"구매한 작품은 자신의 갤러리에 추가됩니다.",
        }
    },
    { u8"실시간",
        {
            u8"각 플레이어는 공개로 가격을 제시합니다.",
            u8"시간이 만료되면 가장 마지막에 최고액을 작성했던 플레이어가 낙찰합니다.",
            u8"구매한 작품은 자신의 갤러리에 추가됩니다.",
        }
    },
    { u8"NTF",
        {
            u8"각 플레이어는 공개로 가격을 제시합니다.",
            u8"시간이 만료되면 가장 마지막에 최고액을 작성했던 플레이어가 낙찰합니다.",
            u8"구매한 작품은 즉시 시세가(랜덤)에 판매됩니다.",
        }
    },
};

Explanation::~Explanation()
{
}

Scene* Explanation::createScene()
{
    return Explanation::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Explanation::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    /////////////////////////////

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto nextRound = lhs::Manager::SingleGameManager::Instance().GetNextRound();

    this->setColor(Color3B::BLACK);

    if (auto title = Label::createWithTTF("Round 1", "fonts/Dovemayo_gothic.ttf", 40))
    {
        title->setPosition({ origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - title->getContentSize().height });
        addChild(title);
    }
    auto lines = scriptMap.at(nextRound);
    std::vector<std::u8string> script
    {
        u8"이번 경매는 " + nextRound + u8" 경매입니다.",
        u8"경매는 작품 하나당 30초간 진행됩니다.",
        lines.at(0),
        lines.at(1),
        lines.at(2),
        u8"가장 높은 금액에 거래된 작가 순으로 작가의 평판이 상승합니다."
    };
    auto dummyAction = CallFunc::create([]() {});
    auto explanationSequence = Sequence::create(dummyAction, nullptr);

    for (int i = 0; i < script.size(); ++i)
    {
        auto delayAction = DelayTime::create(delay);
        auto text = lhs::Utility::ConvertToAscii(script[i]);
        if (auto label = Label::createWithTTF("", "fonts/Dovemayo_gothic.ttf", 24))
        {
            float yTotal = (script.size() + 1) * 2;
            float yPortion = (script.size() - i) * 2 - 1;

            label->setPosition({ origin.x + visibleSize.width / 2,
                                origin.y + 150 + (visibleSize.height - 200) * yPortion / yTotal });
            addChild(label);

            auto updateTextAction = CallFunc::create([=]() { label->setString(text); });
            auto sequence = Sequence::create(delayAction, updateTextAction, delayAction, nullptr);
            explanationSequence = Sequence::create(explanationSequence, sequence, nullptr);
        }
        explanationSequence = Sequence::create(explanationSequence, delayAction, nullptr);
    }
    auto players = lhs::Manager::PlayerManager::Instance().GetRoomPlayers(0);
    auto paintings = lhs::Manager::SingleGameManager::Instance().GetPaintings(4);

    for (auto player : players)
    {
        player->SetPaintings(paintings.back());
        paintings.pop_back();
    }

    auto moveToNextScene = CallFunc::create([=]()
        {
            auto scene = MyGallery::createScene();

            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        });
    explanationSequence = Sequence::create(explanationSequence, moveToNextScene, nullptr);

    runAction(explanationSequence);

    return true;
}


void Explanation::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
