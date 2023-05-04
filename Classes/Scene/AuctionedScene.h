#pragma once

#include "cocos2d.h"

class Auctioned : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(bool isEnd);

    static Auctioned* create(bool isEnd);

    virtual bool init(bool isEnd);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

private:
    bool _isEnd;
};
