#pragma once
#include "cocos2d.h"


class RankingResult : public cocos2d::Scene
{
public:
    ~RankingResult() final;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(RankingResult);

    bool init() final;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
