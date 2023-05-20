#pragma once
#include "cocos2d.h"


class PaintingSubmission : public cocos2d::Scene
{
public:
    ~PaintingSubmission() final;

    static cocos2d::Scene* createScene();

    static PaintingSubmission* create();

    bool init() final;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
