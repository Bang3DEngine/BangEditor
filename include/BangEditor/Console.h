#ifndef CONSOLE_H
#define CONSOLE_H

#include "Bang/Debug.h"
#include "Bang/UIList.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

// ConsoleMessage
class ConsoleMessage
{
public:
    ConsoleMessage() = default;

    DebugMessageType msgType = Undef<DebugMessageType>();
    String msgStr = "";
    int line = -1;
    String fileName = "";
};

// Console
class Console : public GameObject,
                public IDebugListener
{
    GAMEOBJECT_EDITOR(Console);

private:
    UIList *p_messageList = nullptr;
    List<ConsoleMessage> m_messages;

    Console();
    virtual ~Console();

    void AddMessage(const ConsoleMessage &cMsg);

    // GameObject
    void Update() override;

    // IDebugListener
    void OnMessage(DebugMessageType msgType, const String &str,
                   int line, const String &fileName) override;
};

// ConsoleUIListEntry
class ConsoleUIListEntry : public GameObject
{
    GAMEOBJECT_EDITOR(ConsoleUIListEntry);

public:
    ConsoleUIListEntry();
    virtual ~ConsoleUIListEntry();

    void SetConsoleMessage(const ConsoleMessage &cMsg);

private:
    ConsoleMessage m_cMsg;
    UITextRenderer *p_msgText = nullptr;
};

NAMESPACE_BANG_EDITOR_END


#endif // CONSOLE_H
