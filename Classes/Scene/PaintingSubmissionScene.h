#pragma once
#include <cocos2d.h>


class PaintingSubmission : public cocos2d::Scene
{
public:
    ~PaintingSubmission() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(PaintingSubmission);

    bool init() final;
};
