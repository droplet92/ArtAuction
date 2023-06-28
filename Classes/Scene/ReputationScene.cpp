#include "ReputationScene.h"
#include "ExplanationScene.h"
#include "RankingResultScene.h"

#include <algorithm>
#include <format>
#include <ranges>
#include <string>
#include <map>

#include <AudioEngine.h>
USING_NS_CC;

#include <Utility.h>
#include <ScriptSequenceGenerator.h>
#include <Manager/SingleGameManager.h>
using namespace lhs;
using namespace lhs::manager;


constexpr float delay = .3f;

static void ChangeScene(int nextRound)
{
    auto scene = (nextRound < 4)
        ? Explanation::createScene()
        : RankingResult::createScene();
    auto transition = TransitionSlideInB::create(.3f, scene);
    Director::getInstance()->replaceScene(transition);
};

std::vector<std::u8string> GetScript(const std::vector<std::string>& painters, const std::map<std::string, size_t>& reputation)
{
    std::vector<std::u8string> script{};

    for (const auto& painter : painters)
    {
        auto str = std::format("{}: {} golds.", painter, reputation.at(painter));

        script.push_back(Utility::ConvertToUtf8(str));
    }
    return script;
}

Scene* Reputation::createScene()
{
    return Reputation::create();
}

bool Reputation::init()
{
    if (!Scene::init())
        return false;

    // 1. Initialize constants
    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 평판 업데이트
    SingleGameManager::Instance().UpdateReputation();

    const auto [nextRound, _] = SingleGameManager::Instance().GetCurrentRound();
    const auto reputation = SingleGameManager::Instance().GetReputation();
    auto painters = SingleGameManager::Instance().GetPainters();
    
    std::ranges::sort(painters, [=](std::string lhs, std::string rhs)
        {
            return reputation.at(lhs) > reputation.at(rhs);
        });
    const auto script = GetScript(painters, reputation);

    // 2. Create and initialize items
    //
    setColor(Color3B::BLACK);

    auto title = Utility::CreateWithFile<Label>("Reputation", FONT_SIZE_LARGE);
    title->setPosition(origin + Vec2{ visibleSize.width / 2, visibleSize.height - title->getContentSize().height });

    auto scriptSequence = ScriptSequenceGenerator::Generate(script, [=](Label* line) { addChild(line); });
    auto changeSceneAction = CallFunc::create([nextRound]() { ChangeScene(nextRound); });

    // 3. Add items to scene
    //
    addChild(title);
    runAction(Sequence::create(scriptSequence, DelayTime::create(1.f), changeSceneAction, nullptr));

    return true;
}
