#include "Painting.h"

NS_CC_BEGIN
namespace ui
{
    Painting::Painting()
        : image(nullptr)
        , data(nullptr)
    {
        setTouchEnabled(true);
    }

    Painting::~Painting()
    {
    }

    Painting* Painting::create(lhs::Model::Painting const* data)
    {
        auto ret = new (std::nothrow) Painting();
        if (ret && ret->init(data))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool Painting::init(lhs::Model::Painting const* data)
    {
        if (!Layout::init())
            return false;

        this->data = data;

        setContentSize({ 160, 160 });

        if (image = ImageView::create(data->path.string()))
        {
            image->setScale(1 / 15.f);
            image->setPosition({ 80, 80 });
            image->setAnchorPoint({ .5f, .5f });
            addChild(image);

            return true;
        }
        return false;
    }

    void Painting::setScale(float scale)
    {
        setContentSize(getContentSize() * scale);

        image->setScale(image->getScale() * scale);
        image->setPosition(image->getPosition() * scale);
    }

    float Painting::getBottomPadding() const
    {
        auto contentHeight = getContentSize().height;
        auto imageHeight = (image->getContentSize() * image->getScale()).height;

        return (contentHeight - imageHeight) / 2;
    }
}
NS_CC_END
