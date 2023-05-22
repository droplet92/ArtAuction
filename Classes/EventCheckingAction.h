#pragma once
#include <cocos2d.h>


class EventCheckingAction : public cocos2d::ActionInterval
{
public:
    static EventCheckingAction* create(float duration, std::function<void()> eventCallback);

    void update(float time) override;

private:

    void setEventCallback(std::function<void()> eventCallback);

    std::function<void()> eventCallback;
};
