#include "Character.h"
USING_NS_CC;
using namespace ui;

#include <Utility.h>


namespace lhs::widget
{
    constexpr uint8_t ALPHA_OPAQUE = 0xFF;
    constexpr uint8_t ALPHA_TRANSPARENT = 0x00;

    Character::Character()
        : id(0u)
        , state(nullptr)
        , animation(nullptr)
        , bubble(nullptr)
        , board(nullptr)
        , nameTag(nullptr)
    {
    }

    bool Character::init()
    {
        if (!Layout::init())
            return false;

        setContentSize({ 200, 200 });
        setAnchorPoint(Vec2::ZERO);

        state = &CHARACTER_IDLE;
        animation = Utility::Create<Sprite>();

        board = Utility::CreateWithSprite<SpeechBubble>("BidBoard.png");
        board->setPosition({ 0, 130 });
        board->setAnchorPoint({ .5f, 0 });
        board->setOpacity(ALPHA_TRANSPARENT);
        board->SetFixed(true);

        bubble = Utility::CreateWithSprite<SpeechBubble>("bubble.png");
        bubble->setPosition({ 135, 135 });
        bubble->setOpacity(ALPHA_TRANSPARENT);

        addChild(animation);
        addChild(board);
        addChild(bubble);

        return true;
    }

    void Character::SetId(uint32_t id) noexcept
    {
        this->id = id;
    }

    void Character::SetImage(std::string_view filename) noexcept
    {
        animation->initWithSpriteFrameName(filename.data());
        animation->setAnchorPoint({ .5f, 0 });
    }

    void Character::SetName(std::string_view name) noexcept
    {
        nameTag = Utility::CreateWithFile<Text>(name.data());
        nameTag->setAnchorPoint({ .5f, 0 });
        nameTag->enableOutline(Color4B::BLACK, 2);
        addChild(nameTag, 1);
    }

    void Character::Attention() noexcept
    {
        state = &CHARACTER_IDLE;
        state->Enter(this, id, nullptr);
    }

    void Character::Say(std::string_view words) noexcept
    {
        if (state == &CHARACTER_IDLE)
        {
            state = &CHARACTER_SAY;
            state->Enter(this, id, (void*)words.data());
            state = &CHARACTER_IDLE;
        }
    }

    void Character::ShowBid(uint32_t bid)
    {
        // SHOWBID state allowed to facilitate the game's flow.
        if (state == &CHARACTER_IDLE || state == &CHARACTER_SHOWBID)
        {
            state = &CHARACTER_SHOWBID;
            state->Enter(this, id, &bid);
            state = &CHARACTER_IDLE;
        }
    }

    void Character::OpenBid(uint32_t bid)
    {
        if (state == &CHARACTER_IDLE)
        {
            state = &CHARACTER_OPENBID;
            state->Enter(this, id, &bid);
            state = &CHARACTER_IDLE;
        }
    }

    void Character::say(std::string_view words) noexcept
    {
        bubble->SetString(words.data());
        bubble->setOpacity(ALPHA_OPAQUE);
    }

    void Character::stopSaying() noexcept
    {
        bubble->SetString("");
        bubble->setOpacity(ALPHA_TRANSPARENT);
    }

    void Character::showBid(uint32_t bid)
    {
        board->SetString(std::to_string(bid));
        board->setOpacity(ALPHA_OPAQUE);
    }

    void Character::hideBid() noexcept
    {
        board->SetString("");
        board->setOpacity(ALPHA_TRANSPARENT);
    }
}
