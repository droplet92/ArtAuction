#include "Popup.h"

NS_CC_BEGIN

namespace ui
{
	Popup::Popup()
		: _layout(nullptr)
	{
		setTouchEnabled(true);
	}

	Popup::~Popup()
	{
	}

	void Popup::initRenderer()
	{
        //_layout = Layout::create();
        //_buttonNormalRenderer = Scale9Sprite::create();
        //_buttonNormalRenderer->setRenderingType(Scale9Sprite::RenderingType::SIMPLE);

        //addProtectedChild(_buttonNormalRenderer, NORMAL_RENDERER_Z, -1);
	}

    bool Popup::init()
    {
        if (!Widget::init()) {
            return false;
        }

        //loadTextures(normalImage, selectedImage, disableImage, texType);

        return true;
    }
}

NS_CC_END