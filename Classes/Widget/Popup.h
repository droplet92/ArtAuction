#pragma once

#include "ui/UIWidget.h"
#include "ui/GUIExport.h"
#include <ui/UILayout.h>
#include <ui/UIListView.h>
#include <ui/UIButton.h>

/**
 * @addtogroup ui
 * @{
 */
NS_CC_BEGIN

namespace ui
{
    class Popup : public Layout
    {
        DECLARE_CLASS_GUI_INFO

    public:
        Popup();

        ~Popup() override;

        static Popup* create(const Vec2& scale = Vec2::ONE);

        bool init(const Vec2& scale);

        void addContent(Widget* widget);

    private:
        ListView* contents;
        Button* buttonOk;
        Button* buttonCancel;
    };
}

NS_CC_END
// end of ui group
/// @}