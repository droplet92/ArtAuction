#include "BidBoard.h"

#include <format>

#include <AudioEngine.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>
#include <Manager/SingleGameManager.h>
using namespace lhs::manager;


namespace lhs::widget
{
    static constexpr auto MAX_FIELD_LENGTH = 3;

    BidBoard::BidBoard()
        : bidField(nullptr)
        , bidButton(nullptr)
        , popup(nullptr)
        , bidder(nullptr)
        , minBid(nullptr)
    {
    }

    bool BidBoard::init()
    {
        if (!Layout::init())
            return false;

        setAnchorPoint({ .5f, .5f });

        // Resize the background image properly.
        setBackGroundImage("popup.png", Widget::TextureResType::PLIST);
        setBackGroundImageScale9Enabled(true);
        setContentSize({    getBackGroundImageTextureSize().width * .6f,
                            getBackGroundImageTextureSize().height / 3 });

        // Initialize the bidField.
        bidField = Utility::CreateWithFile<TextField>("");
        bidField->setPosition({ 10.f, getContentSize().height / 2 });
        bidField->setAnchorPoint({ 0, .5f });
        bidField->setTextHorizontalAlignment(TextHAlignment::LEFT);
        bidField->setTextColor(Color4B::BLACK);
        bidField->setCursorEnabled(true);
        bidField->setMaxLengthEnabled(true);
        bidField->setMaxLength(MAX_FIELD_LENGTH);
        bidField->addEventListener([](Ref* ref, TextField::EventType type)
            {
                auto field = (TextField*)ref;
                switch (type)
                {
                    // prepare the bidField to a ready state
                case TextField::EventType::ATTACH_WITH_IME: [[fallthrough]];
                case TextField::EventType::DETACH_WITH_IME:
                    field->setString("");
                    break;
                default:
                    break;
                }
            });
        
        // Initialize the bidButton.
        bidButton = Utility::CreateWithSprite<Button>("PopupOk.png");
        bidButton->setPosition({    getContentSize().width - bidButton->getContentSize().width,
                                    getContentSize().height / 2 });
        bidButton->setAnchorPoint({ .1f, .5f });
        bidButton->setTitleFontName(DEFAULT_FONT_NAME);
        bidButton->setTitleFontSize(FONT_SIZE_MEDIUM);
        bidButton->setTitleText("Bid");
        bidButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type)
            {
                if (type != Widget::TouchEventType::BEGAN)
                    return;

                AudioEngine::play2d("audios/click.mp3");

                auto alert = [this](std::string_view message)
                {
                    if (popup->isVisible())
                        return; // alert message already displayed

                    auto text = Utility::CreateWithFile<Text>(message);
                    text->setTextColor(Color4B::BLACK);

                    popup->AddContent(text);
                    popup->setVisible(true);
                };
                try
                {
                    auto offer = std::stoi(bidField->getString());
                    cocos2d::log("%d", offer);

                    // (minimum bid) <= offer <= (bidder's gold)
                    if (offer < *minBid)
                    {
                        alert("Check your offer.");
                    }
                    else if (offer > bidder->GetGold())
                    {
                        alert("Not enough gold.");
                    }
                    else
                    {
                        SingleGameManager::Instance().Bid({ 0, offer });
                        bidField->setPlaceHolder(std::format("Offer: {}", offer));
                        bidField->setString("");
                    }
                }
                // A case that the bid contains invalid characters.
                catch (const std::invalid_argument& e)
                {
                    cocos2d::log("invalid %s: ", e.what());
                    alert("Check your offer.");
                }
                // Unexpected exception.
                catch (const std::exception& e)
                {
                    cocos2d::log("%s: ", e.what());
                    alert("Check your offer.");
                }
            });

        addChild(bidField);
        addChild(bidButton);

        return true;
    }

    void BidBoard::setEnabled(bool enabled)
    {
        bidButton->setEnabled(enabled);
    }

    void BidBoard::RegisterMinimumBid(int& minBid) noexcept
    {
        this->minBid = &minBid;
    }

    void BidBoard::RegisterBidder(model::Player* bidder) noexcept
    {
        this->bidder = bidder;
    }

    void BidBoard::RegisterPopup(Popup* popup) noexcept
    {
        this->popup = popup;
    }

    void BidBoard::UpdateGold()
    {
        auto placeholder = std::format("Gold: {}", bidder->GetGold());
        bidField->setPlaceHolder(placeholder);
    }
}
