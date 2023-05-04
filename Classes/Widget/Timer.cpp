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

    Timer* Timer::create(float time)
    {
        auto timer = new (std::nothrow) Timer{ static_cast<int>(time) };
        if (timer && timer->init(time))
        {
            timer->autorelease();
            return timer;
        }
        CC_SAFE_DELETE(timer);
        return nullptr;
    }

    void Timer::setAlarm(const TimerAlarm& alarm)
    {
        _alarm = alarm;
    }

    std::string Timer::getDescription() const
    {
        return "Timer";
    }

    void Timer::scheduler(float dt)
    {
        if (!_remainTime)
        {
            _alarm(.3f);
            return;
        }
        _time->setString(std::to_string(--_remainTime));
    }

    bool Timer::init(float time)
    {
        if (!Widget::init())
            return false;

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        auto timerSprite = Sprite::create("Timer.png");

        if (!timerSprite)
            return false;

        timerSprite->setContentSize(timerSprite->getContentSize() / 2);

        _time = Label::createWithTTF(std::to_string(_remainTime), "fonts/Dovemayo_gothic.ttf", 60);
        _timer = ProgressTimer::create(timerSprite);

        if (!_time || !_timer)
            return false;

        _time->setTextColor(Color4B::BLACK);

        _timer->setType(ProgressTimer::Type::RADIAL);
        _timer->setReverseDirection(true);

        _timer->runAction(ProgressTo::create(time, 100));
        _timer->schedule([&](float dt) { return this->scheduler(dt); }, 1.f, time, 0, "updateTime");

        auto timerBaseSprite = Sprite::create("TimerBase.png");
        timerBaseSprite->setContentSize(timerBaseSprite->getContentSize() / 2);

        this->addChild(timerBaseSprite);
        this->addChild(_timer);
        this->addChild(_time);

        return true;
    }
}

NS_CC_END