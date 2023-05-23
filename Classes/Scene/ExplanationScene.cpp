#include "ExplanationScene.h"
#include "MyGalleryScene.h"

#include <sstream>
#include <string>
#include <unordered_map>

#include <AudioEngine.h>

#include <Utility.h>
#include <Manager/SingleGameManager.h>

USING_NS_CC;


constexpr float delay = 1.0f;
constexpr float fadeInDuration = 0.5f;

const std::unordered_map<std::u8string, std::vector<std::u8string>> scriptMap =
{
    { u8"Open Bidding",
        {
            u8"Each player offers a price publicly.",
            u8"When the time expires, the last player with the highest bid wins.",
            u8"The purchased artwork is added to their gallery.",
        }
    },
    { u8"Closed Bidding",
        {
            u8"Each player offers a price privately.",
            u8"When the time expires, the first player with the highest bid wins.",
            u8"The purchased artwork is added to their gallery.",
        }
    },
    { u8"Fixed Price",
        {
            u8"Each player offers a price publicly.",
            u8"The first player to write a bid wins.",
            u8"The purchased artwork is added to their gallery.",
        }
    },
    { u8"NFT Open Bidding",
        {
            u8"Each player offers a price publicly.",
            u8"When the time expires, the last player with the highest bid wins.",
            u8"Purchased artwork is immediately sold at market price (randomized).",
        }
    },
    //{ u8"���� ����",
    //    {
    //        u8"�� �÷��̾�� ������ ������ �����մϴ�.",
    //        u8"�ð��� ����Ǹ� ���� �������� �ְ���� �ۼ��ߴ� �÷��̾ �����մϴ�.",
    //        u8"������ ��ǰ�� �ڽ��� �������� �߰��˴ϴ�.",
    //    }
    //},
    //{ u8"����� ����",
    //    {
    //        u8"�� �÷��̾�� ������� ������ �����մϴ�.",
    //        u8"�ð��� ����Ǹ� ���� ���� �ְ���� �ۼ��ߴ� �÷��̾ �����մϴ�.",
    //        u8"������ ��ǰ�� �ڽ��� �������� �߰��˴ϴ�.",
    //    }
    //},
    //{ u8"����",
    //    {
    //        u8"�� �÷��̾�� ������ ������ �����մϴ�.",
    //        u8"���� ���� ���� �ݾ��� �ۼ��ߴ� �÷��̾ �����մϴ�.",
    //        u8"������ ��ǰ�� �ڽ��� �������� �߰��˴ϴ�.",
    //    }
    //},
    //{ u8"NFT ���� ����",
    //    {
    //        u8"�� �÷��̾�� ������ ������ �����մϴ�.",
    //        u8"�ð��� ����Ǹ� ���� �������� �ְ���� �ۼ��ߴ� �÷��̾ �����մϴ�.",
    //        u8"������ ��ǰ�� ��� �ü���(����)�� �Ǹŵ˴ϴ�.",
    //    }
    //},
};

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

    lhs::Manager::SingleGameManager::Instance().MoveToNextRound();
    auto [nextRound, roundType] = lhs::Manager::SingleGameManager::Instance().GetCurrentRound();

    this->setColor(Color3B::BLACK);

    if (auto title = Label::createWithTTF("Round " + std::to_string(nextRound), "fonts/Dovemayo_gothic.ttf", 40))
    {
        title->setPosition({ origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - title->getContentSize().height });
        addChild(title);
    }
    auto lines = scriptMap.at(roundType);
    std::vector<std::u8string> script
    {
        u8"This round is a(n) " + roundType + u8" trade.",
        u8"It lasts 30 seconds per artwork.",
        lines.at(0),
        lines.at(1),
        lines.at(2),
        u8"The artist's reputation increases in order of the highest bidder."
        //u8"�̹� ����� " + roundType + u8" �ŷ��Դϴ�.",
        //u8"��ǰ �ϳ��� 30�ʰ� ����˴ϴ�.",
        //lines.at(0),
        //lines.at(1),
        //lines.at(2),
        //u8"���� ���� �ݾ��� �ŷ��� �۰� ������ �۰��� ������ ����մϴ�."
    };
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
