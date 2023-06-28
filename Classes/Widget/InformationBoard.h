#pragma once
#include <cocos2d.h>
#include <ui/UIListView.h>
#include <ui/UIText.h>


namespace lhs::widget
{
    class InformationBoard : public cocos2d::ui::ListView
    {
    public:
        InformationBoard();

        virtual ~InformationBoard() = default;

        CREATE_FUNC(InformationBoard);
        
        bool init() final;

        void SetTitle(const std::string& title) noexcept;

        void SetInformation(std::string_view data) noexcept;

        void SetInformation(const std::map<std::string, uint32_t>& data) noexcept;

    private:
        cocos2d::ui::Text* title;   // title of the board
    };
}
