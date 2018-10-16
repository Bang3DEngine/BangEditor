#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/Debug.h"
#include "Bang/DebugMessageType.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/List.h"
#include "Bang/Mutex.h"
#include "Bang/String.h"
#include "Bang/UIList.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class IEventsDebug;
FORWARD class UIImageRenderer;
FORWARD class UIList;
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
                public EventListener<IEventsDebug>
{
    GAMEOBJECT_EDITOR(Console);

private:
    Mutex m_mutex;
    UIList *p_messageList = nullptr;
    List<ConsoleMessage> m_messages;
    Array<ConsoleMessage> m_queuedMessages;

    Console();
    virtual ~Console() override;

    void AddMessage(const ConsoleMessage &cMsg);
    void Clear();

    // GameObject
    void Update() override;

    // IEventsDebug
    void OnMessage(DebugMessageType msgType, const String &str,
                   int line, const String &fileName) override;
};

// ConsoleUIListEntry
class ConsoleUIListEntry : public GameObject
{
    GAMEOBJECT_EDITOR(ConsoleUIListEntry);

public:
    ConsoleUIListEntry();
    virtual ~ConsoleUIListEntry() override;

    void SetConsoleMessage(const ConsoleMessage &cMsg);

private:
    ConsoleMessage m_cMsg;
    UITextRenderer *p_msgText = nullptr;
    UIImageRenderer *p_typeIconImg = nullptr;
};

NAMESPACE_BANG_EDITOR_END


#endif // CONSOLE_H
