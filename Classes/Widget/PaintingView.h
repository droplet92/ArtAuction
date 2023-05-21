#pragma once
#include <vector>

#include <cocos2d.h>
#include <ui/CocosGUI.h>

#include <Widget/Painting.h>

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
        ui::Painting* _selected;
    };
}
NS_CC_END
