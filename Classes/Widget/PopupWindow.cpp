#include "PopupWindow.h"

NS_CC_BEGIN

namespace ui
{
	PopupWindow::PopupWindow()
		: _layout(nullptr)
	{
		setTouchEnabled(true);
	}

	PopupWindow::~PopupWindow()
	{
	}

	void PopupWindow::initRenderer()
	{
        //_layout = Layout::create();
        //_buttonNormalRenderer = Scale9Sprite::create();
        //_buttonNormalRenderer->setRenderingType(Scale9Sprite::RenderingType::SIMPLE);

        //addProtectedChild(_buttonNormalRenderer, NORMAL_RENDERER_Z, -1);
	}

    bool PopupWindow::init()
    {
        if (!Widget::init()) {
            return false;
        }

        //loadTextures(normalImage, selectedImage, disableImage, texType);

        return true;
    }
}

NS_CC_END