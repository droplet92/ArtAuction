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

        float getBottomPadding() const;

        void setScale(float scale) override;

        inline lhs::Model::Painting const* getData() const { return data; }

        inline void setColor(const Color3B& color) { image->setColor(color); }

    private:
        ImageView* image;
        lhs::Model::Painting const* data;
    };
}
NS_CC_END
