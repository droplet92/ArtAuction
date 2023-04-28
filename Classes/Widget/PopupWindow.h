#pragma once

#include "ui/UIWidget.h"
#include "ui/GUIExport.h"
#include <ui/UILayout.h>

/**
 * @addtogroup ui
 * @{
 */
NS_CC_BEGIN

namespace ui
{
    class PopupWindow : public Widget
    {
        DECLARE_CLASS_GUI_INFO

    public:
        PopupWindow();

        virtual ~PopupWindow();

        // implement the "static create()" method manually
        CREATE_FUNC(PopupWindow);

    CC_CONSTRUCTOR_ACCESS:
        bool init();

    private:
        virtual void initRenderer() override;

    private:
        Layout* _layout;
    };
}

NS_CC_END
// end of ui group
/// @}