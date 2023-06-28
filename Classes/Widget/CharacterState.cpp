#include "CharacterState.h"
#include "Character.h"
USING_NS_CC;

#include <format>

#include <Utility.h>


namespace lhs::widget
{
    constexpr uint8_t ALPHA_OPAQUE = 0xFF;
    constexpr uint8_t ALPHA_TRANSPARENT = 0x00;

    CallFunc* CreateSayShort(Character* owner, std::string_view words) noexcept
    {
        return CallFunc::create([=]()
            {
                owner->say(words);
            });
    }

    CallFunc* CreateStopSayingShort(Character* owner) noexcept
    {
        return CallFunc::create([=]()
            {
                owner->stopSaying();
            });
    }

    CallFunc* CreateTurnAroundShort(Character* owner, uint32_t id) noexcept
    {
        return CallFunc::create([=]()
            {
                owner->SetImage(std::format("back{}.png", id));
            });
    }

    CallFunc* CreateShowBidShort(Character* owner, uint32_t id, uint32_t bid)
    {
        return CallFunc::create([=]()
            {
                owner->SetImage(std::format("bid{}.png", id));
                owner->showBid(bid);
            });
    }

    CallFunc* CreateTurnForwardShort(Character* owner, uint32_t id) noexcept
    {
        return CallFunc::create([=]()
            {
                owner->SetImage(std::format("front{}.png", id));
                owner->hideBid();
                owner->stopSaying();
            });
    }

    // It takes approximately 0 seconds.
    void CharacterIdle::Enter(Character* owner, uint32_t id, void*) noexcept
    {
        auto animation = CreateTurnForwardShort(owner, id);
        owner->stopAllActions();
        owner->runAction(animation);
    }

    // It takes approximately 1 seconds.
    void CharacterSay::Enter(Character* owner, uint32_t, void* words) noexcept
    {
        const std::string_view data = static_cast<char*>(words);
        auto animation = Sequence::create
        (
            CreateSayShort(owner, data),
            DelayTime::create(1.f),
            CreateStopSayingShort(owner),
            nullptr
        );
        owner->stopAllActions();
        owner->runAction(animation);
    }

    // It takes approximately 1 seconds.
    void CharacterShowBid::Enter(Character* owner, uint32_t id, void* bid)
    {
        const int data = *static_cast<uint32_t*>(bid);
        auto animation = Sequence::create
        (
            CreateShowBidShort(owner, id, data),
            DelayTime::create(1.f),
            CreateTurnForwardShort(owner, id),
            nullptr
        );
        owner->stopAllActions();
        owner->runAction(animation);
    }

    // It takes approximately 9 seconds.
    void CharacterOpenBid::Enter(Character* owner, uint32_t id, void* bid)
    {
        const uint32_t data = *static_cast<uint32_t*>(bid);
        const auto delay = static_cast<float>(id - 1);
        auto animation = Sequence::create
        (
            DelayTime::create(delay),
            CreateTurnAroundShort(owner, id),
            DelayTime::create(4.f),
            CreateShowBidShort(owner, id, data),
            DelayTime::create(5.f - delay),
            CreateTurnForwardShort(owner, id),
            nullptr
        );
        owner->stopAllActions();
        owner->runAction(animation);
    }
}
