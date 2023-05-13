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

        void AddPaintings(std::vector<ui::Painting*> paintings);

        lhs::Model::Painting const* getSelected() const;

    private:
        std::vector<HBox*> _children;
        std::vector<lhs::Model::Painting const*> _data;
        lhs::Model::Painting const* _selected;
    };
}
NS_CC_END
