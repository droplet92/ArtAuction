#pragma once

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <vector>

#include "Widget/Painting.h"

NS_CC_BEGIN
namespace ui
{
    class PaintingView : public VBox
    {
    public:
        PaintingView();
        ~PaintingView() override;

        CREATE_FUNC(PaintingView);

        bool init() override;

    private:
        std::vector<HBox*> _children;
        std::vector<ui::Painting*> _data;
    };
}
NS_CC_END
