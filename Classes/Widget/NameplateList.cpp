#include "NameplateList.h"


namespace lhs::widget
{
    NameplateList::NameplateList()
        : popup(nullptr)
    {
    }

    bool NameplateList::init()
    {
        if (!ListView::init())
            return false;

        setContentSize({ 437.5, 400 });
        setScrollBarEnabled(false);

        return true;
    }

    void NameplateList::RegisterPopup(Popup* popup) noexcept
    {
        this->popup = popup;
    }
}
