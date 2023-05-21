#pragma once
#include <cocos2d.h>


class WaitingRoom : public cocos2d::Scene
{
public:
    ~WaitingRoom() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(WaitingRoom);

    bool init() final;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
