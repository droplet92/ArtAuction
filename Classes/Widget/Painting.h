#pragma once

#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>

#include <filesystem>

NS_CC_BEGIN
namespace ui
{
    class Painting : public Layout
    {
    public:
        ~Painting() override;

        bool init(const std::filesystem::path path);

        static Painting* create(const std::filesystem::path path);

        inline std::string getTitle() const { return title; }
        inline std::string getAuthor() const { return author; }

    private:
        ImageView* image;
        std::string title;
        std::string author;
    };
}
NS_CC_END
