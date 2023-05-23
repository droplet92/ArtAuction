#include "ReputationScene.h"
#include "ExplanationScene.h"
#include "RankingResultScene.h"

#include <algorithm>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>

#include <AudioEngine.h>

#include <Utility.h>
#include <Manager/SingleGameManager.h>

USING_NS_CC;


constexpr float delay = .3f;

Scene* Reputation::createScene()
{
    return Reputation::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Reputation::init()
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

    // 평판 업데이트
    lhs::Manager::SingleGameManager::Instance().UpdateReputation();

    auto [nextRound, roundType] = lhs::Manager::SingleGameManager::Instance().GetCurrentRound();
    auto temp = lhs::Manager::SingleGameManager::Instance().GetPainters();
    auto reputation = lhs::Manager::SingleGameManager::Instance().GetReputation();

    std::vector<std::string> painters{ temp.begin(), temp.end() };
    std::ranges::sort(painters, [=](std::string lhs, std::string rhs)
        {
            return reputation.at(lhs) > reputation.at(rhs);
        });

    this->setColor(Color3B::BLACK);

    if (auto title = Label::createWithTTF("Reputation", "fonts/Dovemayo_gothic.ttf", 40))
    {
        title->setPosition({ origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - title->getContentSize().height });
        addChild(title);
    }
    std::vector<std::u8string> script{};
    for (const auto& painter : painters)
    {
        lhs::u8stringstream ss{};

        ss << lhs::Utility::ConvertToUtf8(painter)
            << u8": "
            << lhs::Utility::ConvertToUtf8(std::to_string(reputation.at(painter)))
            << u8" golds.";
        script.push_back(ss.str());
    }
    auto dummyAction = CallFunc::create({});
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
    auto moveToNextScene = CallFunc::create([=]()
        {
            auto scene = (nextRound < 4)
                ? Explanation::createScene()
                : RankingResult::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
        });
    explanationSequence = Sequence::create(explanationSequence, DelayTime::create(1.f), moveToNextScene, nullptr);

    runAction(explanationSequence);

    return true;
}


void Reputation::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    AudioEngine::end();
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
