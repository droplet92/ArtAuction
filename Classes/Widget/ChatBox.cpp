#include "ChatBox.h"

#include <ui/UIText.h>
USING_NS_CC;
using namespace ui;

#include <Utility.h>


namespace lhs::widget
{
    ChatBox::ChatBox()
        : chatlog(nullptr)
        , chatfield(nullptr)
    {
    }

    bool ChatBox::initWithSpriteFrameName(const std::string& filename)
    {
        if (!Sprite::initWithSpriteFrameName(filename))
            return false;

        chatlog = Utility::Create<ListView>();
        chatlog->setPosition({ 25, 55 });
        chatlog->setContentSize({ 400, 420 });

        chatfield = Utility::CreateWithFile<TextField>("Type a message");
        chatfield->setPosition({ 25, 35 });
        chatfield->setAnchorPoint({ 0, .5f });
        chatfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
        chatfield->setPlaceHolderColor(Color4B::WHITE);
        chatfield->setCursorEnabled(true);
        chatfield->addEventListener([](Ref* ref, TextField::EventType type)
            {
                auto field = dynamic_cast<TextField*>(ref);
                switch (type)
                {
                case TextField::EventType::ATTACH_WITH_IME: // on focus
                    field->setString("");
                    break;
                case TextField::EventType::DETACH_WITH_IME: // out of focus
                    field->setPlaceHolder("Type a message");
                    break;
                default:
                    break;
                }
            });

        addChild(chatlog);
        addChild(chatfield, 1);

        // Add a keyboard event
        auto keyboardEventListener = EventListenerKeyboard::create();
        keyboardEventListener->onKeyReleased = [=](auto code, Event*)
        {
            if (code == EventKeyboard::KeyCode::KEY_ENTER)
            {
                auto chat = Utility::CreateWithFile<Text>(chatfield->getString());

                auto newChatlogHeight = (chatlog->getChildrenCount() + 1) * chat->getContentSize().height;
                auto isScrollAtBottom = newChatlogHeight > chatlog->getInnerContainerSize().height;

                if (isScrollAtBottom)
                {
                    static auto chatSpace = Size{ 0, chat->getContentSize().height };
                    chatlog->setInnerContainerSize(chatlog->getInnerContainerSize() + chatSpace);
                }
                chatlog->addChild(chat);

                if (isScrollAtBottom)
                    chatlog->jumpToBottom();    // focus on new chat

                chatfield->setString("");
            }
            return true;
        };
        auto director = Director::getInstance();
        auto dispatcher = director->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

        return true;
    }
}
