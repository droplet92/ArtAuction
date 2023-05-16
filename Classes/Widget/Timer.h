#pragma once

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/GUIExport.h"
#include <ui/UILayout.h>

/**
 * @addtogroup ui
 * @{
 */
NS_CC_BEGIN

namespace ui
{
    using TimerAlarm = std::function<void()>;

    class Timer : public Layout
    {
        DECLARE_CLASS_GUI_INFO

    public:
        Timer(int remainTime);

        virtual ~Timer();

        static Timer* create(int time);

        bool init();

        void setAlarm(const TimerAlarm& alarm);

        void start();

        void reset(int time);

    private:
        void scheduler(float dt);

    private:
        Label* _time;
        ProgressTimer* _timer;
        TimerAlarm _alarm;

        int _remainTime;
    };
}

NS_CC_END
// end of ui group
/// @}