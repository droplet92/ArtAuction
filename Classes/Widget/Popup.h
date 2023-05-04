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
    class Popup : public Widget
    {
        DECLARE_CLASS_GUI_INFO

    public:
        Popup();

        virtual ~Popup();

        // implement the "static create()" method manually
        CREATE_FUNC(Popup);

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