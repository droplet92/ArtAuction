#include "EventCheckingAction.h"
#include <Manager/SingleGameManager.h>


EventCheckingAction* EventCheckingAction::create(float duration, std::function<void()> eventCallback)
{
    EventCheckingAction* action = new EventCheckingAction();
    action->initWithDuration(duration);
    action->setEventCallback(eventCallback);
    action->autorelease();
    return action;
}

void EventCheckingAction::update(float time)
{
    // 리팩토링할 때 isDone() 호출하는 대신 Action 쪽에서 처리할 것
    if (lhs::Manager::SingleGameManager::Instance().IsBidUpdated() || isDone())
        eventCallback();
}

void EventCheckingAction::setEventCallback(std::function<void()> eventCallback)
{
    this->eventCallback = eventCallback;
}
