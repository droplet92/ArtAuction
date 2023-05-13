#pragma once

#include "cocos2d.h"
#include "Widget/Painting.h"

class PaintingSubmission : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    static PaintingSubmission* create();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
