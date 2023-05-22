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
    // �����丵�� �� isDone() ȣ���ϴ� ��� Action �ʿ��� ó���� ��
    if (lhs::Manager::SingleGameManager::Instance().IsBidUpdated() || isDone())
        eventCallback();
}

void EventCheckingAction::setEventCallback(std::function<void()> eventCallback)
{
    this->eventCallback = eventCallback;
}
