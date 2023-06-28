#include "Painting.h"
USING_NS_CC;
using namespace ui;

#include <Utility.h>


namespace lhs::widget
{
    Painting::Painting()
        : image(nullptr)
    {
        setTouchEnabled(true);
    }

    bool Painting::init()
    {
        if (!Layout::init())
            return false;

        // The size of the Painting(Layout) and the size of the image(ImageView)
        // are designed to have appropriate padding.
        setContentSize({ 190, 190 });

        image = Utility::Create<ImageView>();
        image->setPosition(getContentSize() / 2);
        image->setAnchorPoint({ .5f, .5f });

        addChild(image);

        return true;
    }

    void Painting::setScale(float scale)
    {
        setContentSize(getContentSize() * scale);

        image->setScale(image->getScale() * scale);
        image->setPosition(image->getPosition() * scale);
    }

    float Painting::GetBottomPadding() const noexcept
    {
        auto contentHeight = getContentSize().height;
        auto imageHeight = (image->getContentSize() * image->getScale()).height;

        return (contentHeight - imageHeight) / 2;
    }

    const model::Painting& Painting::GetData() const noexcept
    {
        return data;
    }

    void Painting::SetImage(const model::Painting& data)
    {
        this->data = data;
        image->loadTexture(data.path.string());
        image->setScale(1 / 15.f);  // It makes image size 150x150.
    }

    void Painting::SetColor(const Color3B& color) noexcept
    {
        image->setColor(color);
    }
}
