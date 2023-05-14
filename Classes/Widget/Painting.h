#pragma once

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>

#include <filesystem>
#include <Model/Painting.h>

NS_CC_BEGIN
namespace ui
{
    class Painting : public Layout
    {
    public:
        Painting();
        ~Painting() override;

        static Painting* create(lhs::Model::Painting const* data);

        bool init(lhs::Model::Painting const* data);

        //Widget* createCloneInstance() override;
        //void copyClonedWidgetChildren(Widget* model) override;

        void setScale(float scale) override;

        float getBottomPadding() const;

        inline lhs::Model::Painting const* getData() const { return data; }

    private:
        ImageView* image;
        lhs::Model::Painting const* data;
    };
}
NS_CC_END
