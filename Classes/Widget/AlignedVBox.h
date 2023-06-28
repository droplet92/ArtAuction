#pragma once
#include <cocos2d.h>
#include <ui/UIVBox.h>


namespace lhs::widget
{
    /**
    * The Aligned VBox
    * 
    * The widgets contained by the box are aligned in the center.
    */
    class AlignedVBox : public cocos2d::ui::VBox
    {
    public:
        AlignedVBox();

        virtual ~AlignedVBox() = default;

        CREATE_FUNC(AlignedVBox);

        void AddWidget(cocos2d::ui::Widget* widget);

    private:
        float width;    // Defined as the widest width of widgets
        float height;   // Defined as the sum of heights of widgets
    };
}
