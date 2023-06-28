#include "Timer.h"
USING_NS_CC;

#include <Utility.h>


namespace lhs::widget
{
    Timer::Timer()
        : timer(nullptr)
        , timeLabel(nullptr)
        , alarm({})
        , remainTime(0.f)
    {
        setTouchEnabled(true);
    }

    bool Timer::init()
    {
        if (!ui::Layout::init())
            return false;

        timeLabel = Utility::CreateWithFile<Label>("", FONT_SIZE_LARGE);
        timeLabel->setTextColor(Color4B::BLACK);

        auto timerSprite = Utility::CreateWithSprite<Sprite>("Timer.png");
        timerSprite->setContentSize(timerSprite->getContentSize() / 2);

        timer = ProgressTimer::create(timerSprite);
        timer->setType(ProgressTimer::Type::RADIAL);
        timer->setReverseDirection(true);

        auto timerBaseSprite = Utility::CreateWithSprite<Sprite>("TimerBase.png");
        timerBaseSprite->setContentSize(timerBaseSprite->getContentSize() / 2);

        addChild(timerBaseSprite);
        addChild(timer);
        addChild(timeLabel);

        return true;
    }

    void Timer::SetRemainTime(float time)
    {
        remainTime = time;
        auto timeString = std::to_string(static_cast<int>(time));   // truncate
        timeLabel->setString(timeString);
    }

    void Timer::SetAlarm(const TimerAlarm& alarm) noexcept
    {
        this->alarm = alarm;
    }

    void Timer::Start() noexcept
    {
        auto progress = ProgressTo::create(remainTime, 100.f);
        timer->runAction(progress);
        timer->schedule([=](float) { return scheduler(); }, 1.f, remainTime, 0.f, "updateTime");
    }

    void Timer::Reset(float time)
    {
        // stop countdown
        timer->stopAllActions();
        timer->unscheduleAllCallbacks();

        // reset Label
        SetRemainTime(time);

        // reset ProgressTimer
        timer->setPercentage(0);
        timer->runAction(ProgressTo::create(remainTime, 0.f));
    }

    void Timer::scheduler()
    {
        // done
        if (!remainTime) [[unlikely]]
        {
            if (alarm) alarm();
            return;
        }
        auto timeString = std::to_string(static_cast<int>(--remainTime));   // truncate
        timeLabel->setString(timeString);
    }
}
