#pragma once
#include <cocos2d.h>


class Auction : public cocos2d::Scene
{
public:
    ~Auction() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(Auction);

    bool init() final;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
