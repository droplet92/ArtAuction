#pragma once
#include <cocos2d.h>


class RoomList : public cocos2d::Scene
{
public:
    ~RoomList() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(RoomList);

    bool init() final;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
