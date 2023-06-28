#include "ScriptSequenceGenerator.h"
USING_NS_CC;

#include "Utility.h"


namespace lhs
{
    Sequence* ScriptSequenceGenerator::Generate
    (
        const std::vector<std::u8string>& script,
        std::function<void(Label*)> displayLine
    ) noexcept
    {
        const auto dummyAction = CallFunc::create({});  // for init Sequence
#ifdef _DEBUG   // fast-forward
        const auto delayAction = DelayTime::create(.1f);
#else
        const auto delayAction = DelayTime::create(1.f);
#endif

        const auto visibleSize = Director::getInstance()->getVisibleSize();
        auto scriptSequence = Utility::Create<Sequence>(dummyAction, nullptr);

        for (int i = 0; i < script.size(); ++i)
        {
            // Constants for position y of label
            const float yTotal = (script.size() + 1) * 2;
            const float yPortion = (script.size() - i) * 2 - 1;
            const auto yScale = yPortion / yTotal;

            auto text = Utility::ConvertToAscii(script[i]);
            auto line = Utility::CreateWithFile<Label>("");
            line->setPosition({ visibleSize.width / 2, 150 + (visibleSize.height - 200) * yScale });

            displayLine(line);  // Scene::addChild(line)

            // When updateTextAction is called, the script text is displayed.
            auto updateTextAction = Utility::Create<CallFunc>([=]() { line->setString(text); });
            scriptSequence = Utility::Create<Sequence>
            (
                scriptSequence,
                delayAction,
                updateTextAction,
                delayAction,
                nullptr
            );
        }
        return scriptSequence;
    }
}
