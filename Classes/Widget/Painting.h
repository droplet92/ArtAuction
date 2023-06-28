#pragma once
#include <cocos2d.h>
#include <ui/UILayout.h>
#include <ui/UIImageView.h>

#include <Model/Painting.h>


namespace lhs::widget
{
    class Painting : public cocos2d::ui::Layout
    {
    public:
        Painting();

        ~Painting() final = default;

        CREATE_FUNC(Painting);

        bool init() final;

        void setScale(float scale) override;

        float GetBottomPadding() const noexcept;

        const model::Painting& GetData() const noexcept;

        void SetImage(const model::Painting& data);

        void SetColor(const cocos2d::Color3B& color) noexcept;

    private:
        cocos2d::ui::ImageView* image;  // image display widget
        model::Painting data;
    };
}
