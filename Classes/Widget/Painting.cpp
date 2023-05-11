#include "Painting.h"

NS_CC_BEGIN
namespace ui
{
    Painting::~Painting()
    {
    }

    bool Painting::init(const std::filesystem::path path)
    {
        if (!Layout::init())
            return false;

        setContentSize({ 160, 160 });

        if (image = ImageView::create(path.u8string()))
        {
            title = path.stem().u8string();
            author = title;

            image->setScale(.05f, .05f);
            image->setPosition({ 75, 75 });
            image->setAnchorPoint({ .5f, .5f });
            addChild(image);

            return true;
        }
        return false;
    }

    Painting* Painting::create(const std::filesystem::path path)
    {
        auto ret = new (std::nothrow) Painting();
        if (ret && ret->init(path))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
NS_CC_END
