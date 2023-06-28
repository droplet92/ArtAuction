#pragma once
#include <cocos2d.h>


class Explanation : public cocos2d::Scene
{
public:
    ~Explanation() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(Explanation);

    bool init() final;
};
