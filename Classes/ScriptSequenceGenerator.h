#pragma once
#include <cocos2d.h>


namespace lhs
{
    class ScriptSequenceGenerator
    {
    public:
        /**
        * Generate line-by-line script displaying animation sequence.
        * 
        * script: script with utf-8 strings
        * displayLine: Defines how to add Labels to Scene
        */
        static cocos2d::Sequence* Generate
        (
            const std::vector<std::u8string>& script,
            std::function<void(cocos2d::Label*)> displayLine
        ) noexcept;
    };
}