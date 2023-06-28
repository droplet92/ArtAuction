#pragma once
#include <queue>
#include <string>


namespace lhs
{
    /**
    * The background music player
    * 
    * Singleton pattern used
    */
    class Jukebox
    {
    public:
        static Jukebox& Instance();

        // Loop playback on jukebox
        void Start() noexcept;

    private:
        Jukebox() = default;

        std::queue<std::string> queue;  // BGM queue
    };
}