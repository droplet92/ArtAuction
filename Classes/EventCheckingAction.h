#pragma once
#include <cocos2d.h>


namespace lhs
{
    /**
    * The Action that acts as an event listener
    * 
    * When the bid is completed, it calls the eventCallback.
    * Observer pattern used
    */
    class BidCompleteCheckingAction : public cocos2d::ActionInterval
    {
    public:
        static BidCompleteCheckingAction* create(float duration, std::function<void()> observer);

        /**
        * Check if the bid is completed.
        * 
        * This function works within the cocos2d-x lifecycle.
        */
        void update(float) override;

    private:
        std::function<void()> observer; // bid complete event subscriber
    };
}
