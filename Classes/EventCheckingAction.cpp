#include "EventCheckingAction.h"

#include <Manager/SingleGameManager.h>
using namespace lhs::manager;


namespace lhs
{
    BidCompleteCheckingAction* BidCompleteCheckingAction::create(float duration, std::function<void()> observer)
    {
        BidCompleteCheckingAction* action = new BidCompleteCheckingAction();
        action->initWithDuration(duration);
        action->observer = observer;
        action->autorelease();
        return action;
    }

    void BidCompleteCheckingAction::update(float)
    {
        if (SingleGameManager::Instance().IsBidUpdated() || isDone())
            observer(); // nofity
    }
}
