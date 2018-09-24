#include "BangEditor/UITabContainer.h"

#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/UITabHeader.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UITabContainer::UITabContainer()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *mainVL = AddComponent<UIVerticalLayout>();
    BANG_UNUSED(mainVL);

    p_headersBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *headerBarHL = p_headersBar->AddComponent<UIHorizontalLayout>();
    headerBarHL->SetChildrenVerticalStretch(Stretch::NONE);
    headerBarHL->SetChildrenVerticalAlignment(VerticalAlignment::BOT);

    UILayoutElement *headerBarLE = p_headersBar->AddComponent<UILayoutElement>();
    headerBarLE->SetMinHeight(22);
    headerBarLE->SetLayoutPriority(2);

    GameObject *botPart = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *botPartLE = botPart->AddComponent<UILayoutElement>();
    botPartLE->SetFlexibleSize( Vector2::One );
    botPart->AddComponent<UIVerticalLayout>();

    p_currentTabContainer = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *containerLE =
                    GetCurrentTabContainer()->AddComponent<UILayoutElement>();
    containerLE->SetFlexibleSize(Vector2::One);
    GetCurrentTabContainer()->AddComponent<UIHorizontalLayout>();

    /*
    GameObject *hiddenTabsFocusBlocker = GameObjectFactory::CreateUIGameObject();
    hiddenTabsFocusBlocker->AddComponent<UIFocusable>();
    UIImageRenderer *bg = hiddenTabsFocusBlocker->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::Zero);*/

    p_hiddenTabsContainer = GameObjectFactory::CreateUIGameObject();
    GetHiddenTabsContainer()->AddComponent<UIHorizontalLayout>();
    GetHiddenTabsContainer()->AddComponent<UILayoutIgnorer>();
    GetHiddenTabsContainer()->SetVisible(false);

    p_headersBar->SetParent(this);
    // GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Flexible,
    //                                       1, 1.0f)->SetParent(this);
    botPart->SetParent(this);
    GetHiddenTabsContainer()->SetParent(botPart); // Behind
    // hiddenTabsFocusBlocker->SetParent(botPart); // Between
    GetCurrentTabContainer()->SetParent(botPart); // Front
}

UITabContainer::~UITabContainer()
{
}

void UITabContainer::AddTab(const String &title, GameObject *tabbedChild)
{
    if (!GetChildrenInTabs().Contains(tabbedChild))
    {
        UITabHeader *tabHeader = GameObject::Create<UITabHeader>();
        tabHeader->SetTitle(title);

        tabHeader->SetParent(p_headersBar);
        tabbedChild->SetParent( GetCurrentTabContainer() );

        p_childrenInTabs.PushBack(tabbedChild);
        m_childrenToHeader.Add(tabbedChild, tabHeader);
        m_headerToChildren.Add(tabHeader, tabbedChild);

        tabHeader->EventEmitter<IEventsTabHeader>::RegisterListener(this);

        tabbedChild->SetParent( GetHiddenTabsContainer() );
        if (!GetCurrentTabChild())
        {
            SetCurrentTabChild(tabbedChild);
            tabbedChild->SetVisible(true);
        }
        else { tabbedChild->SetVisible(false); }
    }
}

void UITabContainer::RemoveTab(GameObject *tabbedChild)
{
    p_childrenInTabs.Remove(tabbedChild);

    UITabHeader *header = nullptr;
    if (m_childrenToHeader.ContainsKey(tabbedChild))
    { header = m_childrenToHeader.Get(tabbedChild); }

    m_childrenToHeader.Remove(tabbedChild);
    m_headerToChildren.Remove(header);
}

void UITabContainer::SetTabTitle(GameObject *tabbedChild, const String &title)
{
    if (m_childrenToHeader.ContainsKey(tabbedChild))
    {
        UITabHeader *header = m_childrenToHeader.Get(tabbedChild);
        header->SetTitle(title);
    }
}

GameObject *UITabContainer::GetCurrentTabChild() const
{
    GameObject *currentTabChild = nullptr;
    if (GetCurrentTabIndex() >= 0 &&
        GetCurrentTabIndex() <= SCAST<int>(GetChildrenInTabs().Size()))
    {
        auto it = p_childrenInTabs.Begin();
        std::advance(it, GetCurrentTabIndex());
        currentTabChild = *it;
    }
    return currentTabChild;
}

int UITabContainer::GetCurrentTabIndex() const
{
    return m_currentTabIndex;
}

void UITabContainer::SetCurrentTabIndex(int index)
{
    if (index != GetCurrentTabIndex())
    {
        m_currentTabIndex = index;

        int i = 0;
        for(GameObject *tabbedChild : GetChildrenInTabs())
        {
            UITabHeader *header = m_childrenToHeader.Get(tabbedChild);

            bool front = (i == index);
            GameObject *destContainer = front ? GetCurrentTabContainer() :
                                                GetHiddenTabsContainer();

            header->SetInForeground(front);
            tabbedChild->SetVisible(front);
            tabbedChild->SetParent(destContainer);

            ++i;
        }
    }
}

void UITabContainer::SetCurrentTabChild(GameObject *currentTabChild)
{
    int index = GetChildrenInTabs().IndexOf(currentTabChild);
    SetCurrentTabIndex(index);
}

const List<GameObject*>& UITabContainer::GetChildrenInTabs() const
{
    return p_childrenInTabs;
}

void UITabContainer::OnTabHeaderClicked(UITabHeader *header)
{
    if (m_headerToChildren.ContainsKey(header))
    {
        GameObject *tabbedChild = m_headerToChildren.Get(header);
        SetCurrentTabChild(tabbedChild);
    }

    EventEmitter<IEventsTabHeader>::PropagateToListeners(
                &IEventsTabHeader::OnTabHeaderClicked, header);
}

GameObject *UITabContainer::GetHiddenTabsContainer() const
{
    return p_hiddenTabsContainer;
}

GameObject *UITabContainer::GetCurrentTabContainer() const
{
    return p_currentTabContainer;
}

