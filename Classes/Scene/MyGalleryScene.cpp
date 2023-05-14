#include "ui/CocosGUI.h"
#include <extensions/cocos-ext.h>

#include "MyGalleryScene.h"
#include "PaintingSubmissionScene.h"

#include "Widget/Timer.h"
#include "Widget/Painting.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

#include "Widget/PaintingView.h"

#include <Manager/SingleGameManager.h>
#include <Manager/PlayerManager.h>



USING_NS_CC;
using namespace ui;

Scene* MyGallery::createScene()
{
    return MyGallery::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MyGallery::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add your codes below...

    auto sprite = Sprite::create("backgrounds/5.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'backgrounds/5.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, -1);
    }

    auto roomTitle = Sprite::create("RoomTitle.png");
    if (roomTitle == nullptr)
    {
        problemLoading("'RoomTitle.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        {
            roomTitle->setPosition(Vec2{ origin.x + visibleSize.width / 4,
                                origin.y + visibleSize.height * 1.13f - roomTitle->getContentSize().height });

            // add the sprite as a child to this layer
            this->addChild(roomTitle, -1);

            auto label = Label::createWithTTF("My Gallery", "fonts/Dovemayo_gothic.ttf", 40);
            if (label == nullptr)
            {
                problemLoading("'fonts/Dovemayo_gothic.ttf'");
            }
            else
            {
                label->setAnchorPoint(Vec2{ 0.5, 0.5 });
                label->setTextColor(Color4B::BLACK);
                label->setPosition(Vec2{ roomTitle->getContentSize().width / 2, roomTitle->getContentSize().height / 2 - 15 });
                roomTitle->addChild(label);
            }
        }
    }

    auto roomPlayers = lhs::Manager::PlayerManager::Instance().GetRoomPlayers(0);
    lhs::Manager::SingleGameManager::Instance().SubmitPainting(roomPlayers[1]->GetPainting(1));
    lhs::Manager::SingleGameManager::Instance().SubmitPainting(roomPlayers[2]->GetPainting(2));
    lhs::Manager::SingleGameManager::Instance().SubmitPainting(roomPlayers[3]->GetPainting(3));

    if (auto board = PaintingView::create())
    {
        auto view = roomPlayers[0]->GetPaintings()
            | std::views::transform([](auto data)
                {
                    return ui::Painting::create(data);
                });
        std::vector<ui::Painting*> paintings;
        std::ranges::copy(view, std::back_inserter(paintings));

        board->AddPaintings(paintings);

        board->setBackGroundImage("GalleryBoard.png");
        board->setBackGroundImageScale9Enabled(true);
        board->setContentSize({ 800, 500 });
        board->setAnchorPoint({ .5f, .5f });
        board->setPosition({ visibleSize.width * .4f, visibleSize.height * .4f });

        this->addChild(board);

        if (auto timer = ui::Timer::create(30.f))
        {
            timer->setPosition({ visibleSize.width * .85f, visibleSize.height * .85f });
            timer->setAlarm([=]()
                {
                    auto submission = board->getSelected();

                    lhs::Manager::SingleGameManager::Instance().SubmitPainting(submission);
                    roomPlayers[0]->RemovePainting(submission);

                    auto scene = PaintingSubmission::createScene();

                    Director::getInstance()->replaceScene(TransitionSlideInB::create(.3f, scene));
                });
            timer->play();
            addChild(timer);
        }

        if (auto startButton = Button::create("StartButton.png", "StartButtonPressed.png"))
        {
            startButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                {
                    if (type != ui::Widget::TouchEventType::ENDED)
                        return;

                    auto submission = board->getSelected();

                    lhs::Manager::SingleGameManager::Instance().SubmitPainting(submission);
                    roomPlayers[0]->RemovePainting(submission);

                    auto scene = PaintingSubmission::createScene();

                    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.3, scene));
                });
            startButton->setPosition(Vec2{ origin.x + visibleSize.width * 0.85f, origin.y + visibleSize.height * 0.12f });
            this->addChild(startButton);
        }
    }

    return true;
}


void MyGallery::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
