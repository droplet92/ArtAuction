#include "Jukebox.h"

#include <algorithm>
#include <array>
#include <random>

#include <AudioEngine.h>
USING_NS_CC;


namespace lhs
{
    Jukebox& Jukebox::Instance()
    {
        static Jukebox instance{};

        // This condition only true when this function called at first.
        // And this code block can throw std::exception from std::shuffle() or queue.push().
        // To don't throw exception at ctor and to don't make a new method, used [[unlikely]].
        if (instance.queue.empty()) [[unlikely]]
        {
            std::array<std::string, 3> musics =
            {
                "audios/bgm1.mp3",
                "audios/bgm2.mp3",
                "audios/bgm3.mp3"
            };
            std::random_device rd;
            std::mt19937 gen(rd());

            std::shuffle(std::begin(musics), std::end(musics), gen);

            for (const auto& music : musics)
                instance.queue.push(music);
        }
        return instance;
    }

    void Jukebox::Start() noexcept
    {
        const auto next = queue.front();
        queue.pop();
        queue.push(next);   // It can throw exception but highly unlikely
        int bgmId = AudioEngine::play2d(next, false, .2f);

        AudioEngine::setFinishCallback(bgmId, [=](int, auto) { this->Start(); });
    }
}
