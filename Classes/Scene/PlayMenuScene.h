#pragma once
#include <cocos2d.h>


class PlayMenu : public cocos2d::Scene
{
public:
    ~PlayMenu() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(PlayMenu);

    bool init() final;
};
