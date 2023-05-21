#pragma once
#include <queue>
#include <string>


namespace lhs
{
    class Jukebox
    {
    public:
        static Jukebox& Instance();

        void Start();

    private:
        Jukebox();

        std::queue<std::string> queue;
    };
}