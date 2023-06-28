#pragma once
#include <string_view>

#include <cocos2d.h>
#include <ui/UILayout.h>
#include <ui/UIText.h>

#include <Widget/SpeechBubble.h>
#include <Widget/CharacterState.h>


namespace lhs::widget
{
    /*
    * The Character
    *
    * This class deals with animation, board, and speech bubble using the CharacterState.
    * The Facade pattern is used.
    */
    class Character : public cocos2d::ui::Layout
    {
    public:
        Character();

        virtual ~Character() = default;

        CREATE_FUNC(Character);
        
        bool init() final;

        void SetId(uint32_t id) noexcept;

        void SetImage(std::string_view filename) noexcept;

        void SetName(std::string_view name) noexcept;

        // Reset the animation, the bubble, and the board.
        void Attention() noexcept;

        // Represent words through the bubble.
        void Say(std::string_view words) noexcept;

        // Show the bid directly on the board.
        void ShowBid(uint32_t bid);

        // Open the bid directly on the board with some animation.
        void OpenBid(uint32_t bid);

        // private functions are only accessable by friend functions.
    private:
        void say(std::string_view words) noexcept;

        void stopSaying() noexcept;
        
        void showBid(uint32_t bid);

        void hideBid() noexcept;

        // Character state helpers
        friend cocos2d::CallFunc* CreateSayShort(Character* owner, std::string_view words) noexcept;
        friend cocos2d::CallFunc* CreateStopSayingShort(Character* owner) noexcept;
        friend cocos2d::CallFunc* CreateTurnAroundShort(Character* owner, uint32_t id) noexcept;
        friend cocos2d::CallFunc* CreateShowBidShort(Character* owner, uint32_t id, uint32_t bid);
        friend cocos2d::CallFunc* CreateTurnForwardShort(Character* owner, uint32_t id) noexcept;

    private:
        uint32_t id;                // unique ID
        CharacterState* state;      // character state (FSM)
        cocos2d::Sprite* animation; // character sprite
        SpeechBubble* bubble;       // speech bubble
        SpeechBubble* board;        // bid board
        cocos2d::ui::Text* nameTag; // character name
    };
}
