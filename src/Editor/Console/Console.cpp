#include "BangEditor/Console.h"

#include "Bang/Alignment.h"
#include "Bang/Color.h"
#include "Bang/Debug.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDebug.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/List.tcc"
#include "Bang/Stretch.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIButton.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIList.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"

namespace Bang
{
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

Console::Console()
{
    SetName("Console");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize(Vector2i(100));
    le->SetFlexibleSize(Vector2::One());

    GameObjectFactory::CreateUIGameObjectInto(this);

    GameObject *mainVLGo = this;
    mainVLGo->AddComponent<UIVerticalLayout>();

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

    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 1.0f)
        ->SetParent(toolBar);

    UIButton *clearButton = GameObjectFactory::CreateUIButton("Clear");
    clearButton->AddClickedCallback([this]() { Clear(); });
    clearButton->GetGameObject()->SetParent(toolBar);

    p_messageList = GameObjectFactory::CreateUIList();
    // p_messageList->GetScrollPanel()->SetForceHorizontalFit(false);
    p_messageList->GetScrollPanel()->SetForceHorizontalFit(true);
    p_messageList->GetScrollPanel()->GetScrollArea()->GetBackground()->SetTint(
        Color::White().WithValue(0.7f));
    p_messageList->GetScrollPanel()->SetVerticalScrollBarSide(
        HorizontalSide::RIGHT);
    UILayoutElement *listLE =
        p_messageList->GetGameObject()->AddComponent<UILayoutElement>();
    listLE->SetFlexibleSize(Vector2(1));

    toolBar->SetParent(mainVLGo);
    p_messageList->GetGameObject()->SetParent(mainVLGo);

    Debug::GetInstance()->EventEmitter<IEventsDebug>::RegisterListener(this);
}

Console::~Console()
{
}

void Console::Update()
{
    GameObject::Update();

    for (const ConsoleMessage &cMsg : m_queuedMessages)
    {
        ConsoleUIListEntry *entryGo = new ConsoleUIListEntry();
        entryGo->SetConsoleMessage(cMsg);
        p_messageList->AddItem(entryGo);
        m_messages.PushBack(cMsg);
        p_messageList->ScrollToEnd();
    }
    m_queuedMessages.Clear();
}

void Console::AddMessage(const ConsoleMessage &cMsg)
{
    m_mutex.Lock();
    m_queuedMessages.PushBack(cMsg);
    m_mutex.UnLock();
}

void Console::Clear()
{
    m_messages.Clear();
    p_messageList->Clear();
}

void Console::OnMessage(DebugMessageType msgType,
                        const String &str,
                        int line,
                        const String &fileName)
{
    if (msgType != DebugMessageType::DLOG)
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
    hl->SetChildrenHorizontalStretch(Stretch::NONE);
    hl->SetChildrenVerticalStretch(Stretch::NONE);
    hl->SetChildrenVerticalAlignment(VerticalAlignment::TOP);
    hl->SetPaddings(5);
    hl->SetSpacing(10);

    GameObject *typeIconGo = GameObjectFactory::CreateUIGameObject();
    p_typeIconImg = typeIconGo->AddComponent<UIImageRenderer>();
    p_typeIconImg->SetImageTexture(TextureFactory::GetWarningIcon());
    UILayoutElement *iconLE = typeIconGo->AddComponent<UILayoutElement>();
    iconLE->SetMinSize(Vector2i(20));
    iconLE->SetFlexibleSize(Vector2::Zero());

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    p_msgText = textGo->AddComponent<UITextRenderer>();
    p_msgText->SetVerticalAlign(VerticalAlignment::TOP);
    p_msgText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    p_msgText->SetWrapping(true);
    p_msgText->SetTextSize(12);
    UILayoutElement *textLE = textGo->AddComponent<UILayoutElement>();
    textLE->SetFlexibleSize(Vector2::One());

    typeIconGo->SetParent(this);
    textGo->SetParent(this);
}

ConsoleUIListEntry::~ConsoleUIListEntry()
{
}

void ConsoleUIListEntry::SetConsoleMessage(const ConsoleMessage &cMsg)
{
    m_cMsg = cMsg;
    p_msgText->SetContent(cMsg.msgStr);

    Texture2D *iconTex = nullptr;
    switch (cMsg.msgType)
    {
        case DebugMessageType::LOG:
        case DebugMessageType::DLOG:
            iconTex = TextureFactory::GetInfoIcon();
            break;
        case DebugMessageType::WARN:
            iconTex = TextureFactory::GetWarningIcon();
            break;
        case DebugMessageType::ERROR:
            iconTex = TextureFactory::GetErrorIcon();
            break;
    }
    p_typeIconImg->SetImageTexture(iconTex);
}
