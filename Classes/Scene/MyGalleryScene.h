#pragma once
#include <cocos2d.h>


class MyGallery : public cocos2d::Scene
{
public:
    ~MyGallery() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(MyGallery);

    bool init() final;
};
