#pragma once
#include <cocos2d.h>
#include <Model/Painting.h>


class Auction : public cocos2d::Scene
{
public:
    Auction();

    ~Auction() final = default;

    static cocos2d::Scene* createScene();

    // implement the "static create()" method manually
    CREATE_FUNC(Auction);

    bool init() final;

private:
    std::vector<cocos2d::Action*> gamePlayActions;
    std::vector<std::pair<int, int>> bids;
    int minBid;
    lhs::model::Painting selection;
    std::string winnerMessage;
    std::string startMessage;
    std::string bidMessage;
};
