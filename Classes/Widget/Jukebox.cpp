#include "Jukebox.h"

#include <sstream>

#include <AudioEngine.h>
#include <ccRandom.h>

USING_NS_CC;


namespace lhs
{
    Jukebox& Jukebox::Instance()
    {
        static Jukebox* instance = new Jukebox;
        return *instance;
    }

    void Jukebox::Start()
    {
        auto next = queue.front();
        queue.pop();
        queue.push(next);
        int bgmId = AudioEngine::play2d(next, false, .2f);

        AudioEngine::setFinishCallback(bgmId, [=](int, auto) { this->Start(); });
    }

    Jukebox::Jukebox()
    {
        AudioEngine::preload("audios/bgm1.mp3");
        AudioEngine::preload("audios/bgm2.mp3");
        AudioEngine::preload("audios/bgm3.mp3");

        int id = RandomHelper::random_int(1, 3);

        for (int i = 0; i < 3; i++)
        {
            std::stringstream nextPath{};
            id = (id % 3) + 1;

            nextPath << "audios/bgm" << id << ".mp3";
            queue.push(nextPath.str());
        }
    }
}
