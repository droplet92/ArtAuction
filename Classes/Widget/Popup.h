#pragma once
#include <ui/UIButton.h>
#include <ui/UILayout.h>
#include <ui/UIListView.h>

NS_CC_BEGIN
namespace ui
{
    class Popup : public Layout
    {
        DECLARE_CLASS_GUI_INFO

    public:
        Popup();

        ~Popup() override;

        static Popup* create(const Vec2& scale = Vec2::ONE, bool canCancel = true);

        bool init(const Vec2& scale, bool canCancel);

        void addContent(Widget* widget);

    private:
        ListView* contents;
        Button* buttonOk;
        Button* buttonCancel;
    };
}
NS_CC_END
