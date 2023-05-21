#pragma once
#include <cocos2d.h>


class Title : public cocos2d::Scene
{
public:
    ~Title() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(Title);

    bool init() final;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
};
