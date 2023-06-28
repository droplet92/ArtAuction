#pragma once
#include <cocos2d.h>
#include <ui/UILayout.h>


namespace lhs::widget
{
    using TimerAlarm = std::function<void()>;

    /**
    * The timeout event handler
    * 
    * The Observer patter is used.
    */
    class Timer : public cocos2d::ui::Layout
    {
    public:
        Timer();

        virtual ~Timer() = default;

        CREATE_FUNC(Timer);

        bool init() final;

        void SetRemainTime(float time);

        void SetAlarm(const TimerAlarm& alarm) noexcept;

        void Start() noexcept;

        void Reset(float time);

    private:
        void scheduler();

    private:
        cocos2d::ProgressTimer* timer;  // real timer
        cocos2d::Label* timeLabel;      // time string
        TimerAlarm alarm;               // timeout observer
        float remainTime;
    };
}
