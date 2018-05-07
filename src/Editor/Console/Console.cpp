#include "BangEditor/Console.h"

#include "Bang/Thread.h"
#include "Bang/UIList.h"
#include "Bang/Application.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Console::Console()
{
    SetName("Console");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIRendererCacher *rendCacher = GameObjectFactory::CreateUIRendererCacherInto(this);
    GameObject *rendererCacherContainer = rendCacher->GetContainer();

    GameObject *mainVLGo = rendererCacherContainer;
    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>();

    // Tool Bar
    GameObject *toolBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolBarHL = toolBar->AddComponent<UIHorizontalLayout>();
    toolBarHL->SetPaddingTop(3);
    toolBarHL->SetPaddingBot(3);
    toolBarHL->SetPaddingLeft(3);
    toolBarHL->SetPaddingRight(3);
    toolBarHL->SetSpacing(3);

    constexpr int ToolBarHeight = 30;
    UILayoutElement *toolBarLE = toolBar->AddComponent<UILayoutElement>();
    toolBarLE->SetMinHeight(ToolBarHeight);
    toolBarLE->SetFlexibleHeight(0);

    p_messageList = GameObjectFactory::CreateUIList();
    // p_messageList->GetScrollPanel()->SetForceHorizontalFit(false);
    p_messageList->GetScrollPanel()->SetForceHorizontalFit(true);
    UILayoutElement *listLE = p_messageList->GetGameObject()->
                              AddComponent<UILayoutElement>();
    listLE->SetFlexibleSize( Vector2(1) );

    toolBar->SetParent(mainVLGo);
    p_messageList->GetGameObject()->SetParent(mainVLGo);

    Debug::GetInstance()->EventEmitter<IDebugListener>::RegisterListener(this);
}

Console::~Console()
{

}

void Console::Update()
{
    GameObject::Update();
}

void Console::AddMessage(const ConsoleMessage &cMsg)
{
    ConsoleUIListEntry *entryGo = GameObject::Create<ConsoleUIListEntry>();
    entryGo->SetConsoleMessage(cMsg);
    p_messageList->AddItem(entryGo);

    m_messages.PushBack(cMsg);
}

void Console::OnMessage(DebugMessageType msgType, const String &str,
                        int line, const String &fileName)
{
    bool isMainThread = (Thread::GetCurrentThreadId() ==
                         Application::GetMainThreadId());
    if (isMainThread)
    {
        ConsoleMessage cMsg;
        cMsg.msgType = msgType;
        cMsg.msgStr = str;
        cMsg.line = line;
        cMsg.fileName = fileName;

        AddMessage(cMsg);
    }
}


// ConsoleUIListEntry
ConsoleUIListEntry::ConsoleUIListEntry()
{
    SetName("ConsoleUIListEntry");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalStretch(Stretch::Full);
    hl->SetChildrenVerticalStretch(Stretch::Full);
    hl->SetPaddings(5);
    hl->SetSpacing(10);

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    p_msgText = textGo->AddComponent<UITextRenderer>();
    p_msgText->SetVerticalAlign(VerticalAlignment::Top);
    p_msgText->SetHorizontalAlign(HorizontalAlignment::Left);
    p_msgText->SetWrapping(true);
    p_msgText->SetTextSize(12);

    textGo->SetParent(this);
}

ConsoleUIListEntry::~ConsoleUIListEntry()
{
}

void ConsoleUIListEntry::SetConsoleMessage(const ConsoleMessage &cMsg)
{
    m_cMsg = cMsg;
    p_msgText->SetContent(cMsg.msgStr);
}
