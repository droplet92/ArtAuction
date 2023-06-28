#pragma once
#include <cocos2d.h>


class RankingResult : public cocos2d::Scene
{
public:
    ~RankingResult() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(RankingResult);

    bool init() final;
};
