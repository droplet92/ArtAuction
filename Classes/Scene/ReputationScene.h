#pragma once
#include <cocos2d.h>


class Reputation : public cocos2d::Scene
{
public:
    ~Reputation() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(Reputation);

    bool init() final;
};
