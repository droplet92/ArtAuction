#pragma once
#include <concepts>
#include <string_view>

#include <cocos2d.h>
#include <ui/UIButton.h>
#include <ui/UIListView.h>

#include "Widget/Character.h"


namespace lhs
{
    // The concept implements create function
    template <class T>
    concept Creatable = requires(T)
    {
        { T::create() } -> std::same_as<T*>;
    };

    // The concept implements TTF functions
    template <class T>
    concept TextUI = requires(T x)
    {
        { x.setString("") } -> std::same_as<void>;
        { x.setFontName("") } -> std::same_as<void>;
        { x.setFontSize(0) } -> std::same_as<void>;
    };

    // Default font style
    constexpr auto DEFAULT_FONT_NAME = "fonts/Dovemayo_gothic.ttf";

    // Font sizes
    constexpr auto FONT_SIZE_SMALL = 16.f;
    constexpr auto FONT_SIZE_MEDIUM = 24.f;
    constexpr auto FONT_SIZE_LARGE = 40.f;

    // The message displayed when the create() function call fails.
    static constexpr auto OBJECT_CREATION_FAILED_MESSAGE = "The program failed to create an object.\nThis may be due to an out-of-memory problem.";

    // The message displayed when the initWithFile() style function call fails.
    static constexpr auto FILE_LOAD_FAILED_MESSAGE = "The program failed to load a data file.\nPlease check integrity of game files.";

	class Utility
	{
	public:
        /**
        * Wrapper of std::abort
        * 
        * Display window error message and exit program.
        */
        [[noreturn]] static void Abort(std::string_view message) noexcept;

        /**
        * Wrappers of create functions
        * 
        * Create an object and add it to the cocos2d-x memory pool.
        * If create() fails, abort the program.
        */
        template <class Creatable>
        static Creatable* Create() noexcept
        {
            auto* borrow = Creatable::create();

            if (!borrow)
                Abort(OBJECT_CREATION_FAILED_MESSAGE);

            return borrow;
        }

        template <class T>
            requires std::derived_from<T, cocos2d::CallFunc>
        static T* Create(const std::function<void()>& func) noexcept
        {
            T* borrow = T::create(func);

            if (!borrow)
                Abort(OBJECT_CREATION_FAILED_MESSAGE);

            return borrow;
        }

        template <class T, typename... Args>
            requires std::derived_from<T, cocos2d::Sequence>
        static T* Create(cocos2d::FiniteTimeAction* action1, Args... args) noexcept
        {
            T* borrow = T::create(action1, args...);

            if (!borrow)
                Abort(OBJECT_CREATION_FAILED_MESSAGE);

            return borrow;
        }

        template <class T>
            requires Creatable<T> && std::derived_from<T, cocos2d::Label>
        static T* CreateWithFile
        (
            std::string_view message,
            float fontSize = FONT_SIZE_MEDIUM,
            std::string_view fontName = DEFAULT_FONT_NAME
        ) noexcept
        {
            auto* borrow = Create<T>();

            if (!borrow->initWithTTF(message.data(), fontName.data(), fontSize))
                Abort(FILE_LOAD_FAILED_MESSAGE);

            return borrow;
        }

        template <class T>
            requires Creatable<T>&& TextUI<T>
        static T* CreateWithFile
        (
            std::string_view message,
            float fontSize = FONT_SIZE_MEDIUM,
            std::string_view fontName = DEFAULT_FONT_NAME
        ) noexcept
        {
            auto* borrow = Create<T>();

            borrow->setString(message.data());
            borrow->setFontName(fontName.data());
            borrow->setFontSize(fontSize);
            return borrow;
        }

        template <class T>
            requires Creatable<T> && std::derived_from<T, cocos2d::Sprite>
        static T* CreateWithFile(std::string_view fileName) noexcept
        {
            auto* borrow = Create<T>();

            if (!borrow->initWithFile(fileName.data()))
                Abort(FILE_LOAD_FAILED_MESSAGE);

            return borrow;
        }

        template <class T>
            requires Creatable<T> && std::derived_from<T, cocos2d::Sprite>
        static T* CreateWithSprite(std::string_view fileName) noexcept
        {
            auto* borrow = Create<T>();

            if (!borrow->initWithSpriteFrameName(fileName.data()))
                Abort(FILE_LOAD_FAILED_MESSAGE);

            return borrow;
        }

        template <class T>
            requires Creatable<T> && std::derived_from<T, cocos2d::ui::Button>
        static T* CreateWithSprite(std::string_view normal, std::string_view pressed = "") noexcept
        {
            auto* borrow = Create<T>();

            borrow->loadTextureNormal(normal.data(), T::TextureResType::PLIST);
            borrow->loadTexturePressed(pressed.data(), T::TextureResType::PLIST);
            return borrow;
        }

        template <class T>
            requires Creatable<T>&& std::derived_from<T, cocos2d::ui::ListView>
        static T* CreateWithSprite(std::string_view fileName) noexcept
        {
            auto* borrow = Create<T>();

            borrow->setBackGroundImage(fileName.data(), T::TextureResType::PLIST);
            borrow->setContentSize(borrow->getBackGroundImageTextureSize());
            return borrow;
        }

        // Ascii-UTF8 Converters
		static std::string ConvertToAscii(std::u8string_view u8str) noexcept;
		static std::u8string ConvertToUtf8(std::string_view str) noexcept;
	};
}
