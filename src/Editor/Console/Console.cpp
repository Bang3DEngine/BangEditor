#include "BangEditor/Console.h"

#include "Bang/Thread.h"
#include "Bang/UIList.h"
#include "Bang/UIButton.h"
#include "Bang/Application.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/TextureFactory.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/EditorTextureFactory.h"

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

    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 1.0f)->
                       SetParent(toolBar);

    UIButton *clearButton = GameObjectFactory::CreateUIButton("Clear");
    clearButton->AddClickedCallback([this]() { Clear(); });
    clearButton->GetGameObject()->SetParent(toolBar);

    p_messageList = GameObjectFactory::CreateUIList();
    // p_messageList->GetScrollPanel()->SetForceHorizontalFit(false);
    p_messageList->GetScrollPanel()->SetForceHorizontalFit(true);
    p_messageList->GetScrollPanel()->SetVerticalScrollBarSide(HorizontalSide::RIGHT);
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
    p_messageList->ScrollToEnd();
}

void Console::Clear()
{
    m_messages.Clear();
    p_messageList->Clear();
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
    hl->SetChildrenHorizontalStretch(Stretch::NONE);
    hl->SetChildrenVerticalStretch(Stretch::NONE);
    hl->SetChildrenVerticalAlignment(VerticalAlignment::TOP);
    hl->SetPaddings(5);
    hl->SetSpacing(10);

    GameObject *typeIconGo = GameObjectFactory::CreateUIGameObject();
    p_typeIconImg = typeIconGo->AddComponent<UIImageRenderer>();
    p_typeIconImg->SetImageTexture( TextureFactory::GetWarningIcon().Get() );
    UILayoutElement *iconLE = typeIconGo->AddComponent<UILayoutElement>();
    iconLE->SetMinSize( Vector2i(20) );
    iconLE->SetFlexibleSize( Vector2::Zero );

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    p_msgText = textGo->AddComponent<UITextRenderer>();
    p_msgText->SetVerticalAlign(VerticalAlignment::TOP);
    p_msgText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    p_msgText->SetWrapping(true);
    p_msgText->SetTextSize(12);
    UILayoutElement *textLE = textGo->AddComponent<UILayoutElement>();
    textLE->SetFlexibleSize( Vector2::One );

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
            iconTex = TextureFactory::GetInfoIcon().Get();
        break;
        case DebugMessageType::WARN:
            iconTex = TextureFactory::GetWarningIcon().Get();
        break;
        case DebugMessageType::ERROR:
            iconTex = TextureFactory::GetErrorIcon().Get();
        break;
    }
    p_typeIconImg->SetImageTexture(iconTex);
}
