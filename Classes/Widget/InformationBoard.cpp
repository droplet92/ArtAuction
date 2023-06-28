#include "InformationBoard.h"

#include <ui/UIRichText.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>


namespace lhs::widget
{
    constexpr uint8_t ALPHA_OPAQUE = 0xFF;
    const Color4B COLOR_MAIN = { 0x58, 0xCD, 0xF8, ALPHA_OPAQUE };
    const Color3B COLOR_ACCENT = { 0xF8, 0xE2, 0x57 };

    InformationBoard::InformationBoard()
        : title(nullptr)
    {
    }

    bool InformationBoard::init()
    {
        if (!ListView::init())
            return false;

        setAnchorPoint({ .5f, .1f });

        title = Utility::CreateWithFile<Text>("");
        title->setTextColor(COLOR_MAIN);
        
        addChild(title);

        return true;
    }

    void InformationBoard::SetTitle(const std::string& title) noexcept
    {
        this->title->setString(title);
    }

    void InformationBoard::SetInformation(std::string_view data) noexcept
    {
        if (getChildByTag(0xCAFEF00D))
            removeChildByTag(0xCAFEF00D);

        auto text = Utility::CreateWithFile<Text>(data, FONT_SIZE_SMALL);
        text->setTextAreaSize({ getContentSize().width - 20, 100 });

        setPadding(20.f, 20.f, 20.f, 20.f);
        setItemsMargin(10);

        addChild(text, 0, 0xCAFEF00D);
    }

    void InformationBoard::SetInformation(const std::map<std::string, uint32_t>& data) noexcept
    {
        // Sort the data in descending order of value
        std::vector<std::pair<uint32_t, std::string>> temp{};

        for (auto& [key, value] : data)
            temp.push_back(std::make_pair(value, key));

        std::sort(std::begin(temp), std::end(temp), std::greater<>());

        // Add texts
        for (const auto& [value, painter] : temp)
        {
            // painters are unique key
            if (getChildByName(painter))
                removeChildByName(painter);

            auto text = Utility::Create<RichText>();
            auto painterText = RichElementText::create(1, Color3B::WHITE, ALPHA_OPAQUE, painter + ": ", DEFAULT_FONT_NAME, FONT_SIZE_SMALL);
            auto nPiecesText = RichElementText::create(2, COLOR_ACCENT, ALPHA_OPAQUE, std::to_string(value), DEFAULT_FONT_NAME, FONT_SIZE_SMALL);

            text->ignoreContentAdaptWithSize(false);
            text->setContentSize({ 200, 20 });
            text->pushBackElement(painterText);
            text->pushBackElement(nPiecesText);

            addChild(text, 0, painter);
        }
        setPadding(10.f, 10.f, 10.f, 10.f);
    }
}
