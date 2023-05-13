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
            image->setScale(.05f, .05f);
            image->setPosition({ 75, 75 });
            image->setAnchorPoint({ .5f, .5f });
            addChild(image);

            return true;
        }
        return false;
    }

    Widget* Painting::createCloneInstance()
    {
        return Painting::create(data);
    }

    void Painting::copyClonedWidgetChildren(Widget* model)
    {
        // image 2�� ���� ����
    }

    void Painting::setScale(float scale)
    {
        setContentSize(getContentSize() * scale);

        image->setScale(image->getScale() * scale);
        image->setPosition(image->getPosition() * scale);
    }
}
NS_CC_END
