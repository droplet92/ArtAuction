#pragma once
#include <cocos2d.h>
#include <ui/UIVBox.h>
#include <ui/UIHBox.h>

#include <Widget/Painting.h>


namespace lhs::widget
{
    /**
    * The table view of paintings
    * 
    * It shows paintings in a 5xN table format.
    */
    class PaintingView : public cocos2d::ui::VBox
    {
    public:
        PaintingView();

        ~PaintingView() final = default;

        CREATE_FUNC(PaintingView);

        void AddPaintings(const std::vector<Painting*>& paintings);

        const model::Painting& GetSelected() const;

    private:
        std::vector<cocos2d::ui::HBox*> children;   // rows
        std::vector<model::Painting> data;          // paintings
        Painting* selected;     // selected painting by player
    };
}
