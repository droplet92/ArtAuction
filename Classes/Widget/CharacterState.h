#pragma once
#include <cstdint>


namespace lhs::widget
{
    class Character;    // forward declaration

    /*
    * The state of the Character
    * 
    * It deals with animation, board, and speech bubble using private functions of Character.
    * The State pattern is used.
    */
    class CharacterState
    {
    public:
        virtual ~CharacterState() = default;

        virtual void Enter(Character* owner, uint32_t id, void* data) = 0;
    };

    class CharacterIdle : public CharacterState
    {
    public:
        ~CharacterIdle() final = default;

        void Enter(Character* owner, uint32_t id, void* data) noexcept final;
    };

    class CharacterSay : public CharacterState
    {
    public:
        ~CharacterSay() final = default;

        void Enter(Character* owner, uint32_t id, void* words) noexcept final;
    };

    class CharacterOpenBid : public CharacterState
    {
    public:
        ~CharacterOpenBid() final = default;

        void Enter(Character* owner, uint32_t id, void* bid) final;
    };

    class CharacterShowBid : public CharacterState
    {
    public:
        ~CharacterShowBid() final = default;

        void Enter(Character* owner, uint32_t id, void* bid) final;
    };

    static CharacterIdle CHARACTER_IDLE{};
    static CharacterSay CHARACTER_SAY{};
    static CharacterShowBid CHARACTER_SHOWBID{};
    static CharacterOpenBid CHARACTER_OPENBID{};
}
