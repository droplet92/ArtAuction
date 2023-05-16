#include "Timer.h"

NS_CC_BEGIN

namespace ui
{
	Timer::Timer(int remainTime)
		: _time(nullptr)
        , _timer(nullptr)
        , _remainTime(remainTime)
	{
		setTouchEnabled(true);
	}

	Timer::~Timer()
	{
	}

    Timer* Timer::create(int time)
    {
        auto timer = new (std::nothrow) Timer{ static_cast<int>(time) };
        if (timer && timer->init())
        {
            timer->autorelease();
            return timer;
        }
        CC_SAFE_DELETE(timer);
        return nullptr;
    }

    bool Timer::init()
    {
        if (!Widget::init())
            return false;

        auto timerSprite = Sprite::create("Timer.png");

        if (!timerSprite)
            return false;

        timerSprite->setContentSize(timerSprite->getContentSize() / 2);

        _time = Label::createWithTTF(std::to_string(_remainTime), "fonts/Dovemayo_gothic.ttf", 60);
        _timer = ProgressTimer::create(timerSprite);

        _time->setTextColor(Color4B::BLACK);
        _timer->setType(ProgressTimer::Type::RADIAL);
        _timer->setReverseDirection(true);

        auto timerBaseSprite = Sprite::create("TimerBase.png");
        timerBaseSprite->setContentSize(timerBaseSprite->getContentSize() / 2);

        addChild(timerBaseSprite);
        addChild(_timer);
        addChild(_time);

        return true;
    }

    void Timer::setAlarm(const TimerAlarm& alarm)
    {
        _alarm = alarm;
    }

    void Timer::start()
    {
        _timer->runAction(ProgressTo::create(_remainTime, 100));
        _timer->schedule([&](float dt) { return this->scheduler(dt); }, 1.f, _remainTime, 0, "updateTime");
    }

    void Timer::reset(int time)
    {
        _remainTime = time;

        _time->setString(std::to_string(time));
        _timer->setPercentage(0);
        _timer->runAction(ProgressTo::create(_remainTime, 0));
    }

    std::string Timer::getDescription() const
    {
        return "Timer";
    }

    void Timer::scheduler(float dt)
    {
        if (!_remainTime)
        {
            _alarm();
            return;
        }
        _time->setString(std::to_string(--_remainTime));
    }
}

NS_CC_END
