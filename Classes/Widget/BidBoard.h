#pragma once
#include <cocos2d.h>
#include <ui/UILayout.h>
#include <ui/UITextField.h>
#include <ui/UIButton.h>

#include <Model/Player.h>
#include <Widget/Popup.h>


namespace lhs::widget
{
    class BidBoard : public cocos2d::ui::Layout
    {
    public:
        BidBoard();

        virtual ~BidBoard() = default;

        CREATE_FUNC(BidBoard);
        
        bool init() final;

        void setEnabled(bool enabled) final;

        void RegisterMinimumBid(int& minBid) noexcept;

        void RegisterBidder(model::Player* bidder) noexcept;

        void RegisterPopup(Popup* popup) noexcept;

        void UpdateGold();

    private:
        cocos2d::ui::TextField* bidField;
        cocos2d::ui::Button* bidButton;
        int* minBid;            // minimum bid
        Popup* popup;           // alert popup
        model::Player* bidder;  // player
    };
}
