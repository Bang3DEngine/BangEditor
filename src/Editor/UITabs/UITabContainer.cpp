#include "BangEditor/UITabContainer.h"

#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
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

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize(Vector2::One);

    p_headersBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *headerBarHL = GetHeadersBar()->AddComponent<UIHorizontalLayout>();
    headerBarHL->SetChildrenVerticalStretch(Stretch::NONE);
    headerBarHL->SetChildrenVerticalAlignment(VerticalAlignment::BOT);

    // UIImageRenderer *barBg = GetHeadersBar()->AddComponent<UIImageRenderer>();
    // barBg->SetTint(Color::White.WithValue(0.4f));

    UILayoutElement *headerBarLE = GetHeadersBar()->AddComponent<UILayoutElement>();
    headerBarLE->SetMinHeight(22);
    headerBarLE->SetLayoutPriority(2);

    GameObject *botPart = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *botPartLE = botPart->AddComponent<UILayoutElement>();
    botPartLE->SetFlexibleSize( Vector2::One );
    botPart->AddComponent<UIVerticalLayout>();

    p_currentTabContainer = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *containerLE =
                    GetVisibleContainer()->AddComponent<UILayoutElement>();
    containerLE->SetFlexibleSize(Vector2::One);
    GetVisibleContainer()->AddComponent<UIHorizontalLayout>();

    /*
    GameObject *hiddenTabsFocusBlocker = GameObjectFactory::CreateUIGameObject();
    hiddenTabsFocusBlocker->AddComponent<UIFocusable>();
    UIImageRenderer *bg = hiddenTabsFocusBlocker->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::Zero);*/

    p_hiddenTabsContainer = GameObjectFactory::CreateUIGameObject();
    GetHiddenContainer()->AddComponent<UIHorizontalLayout>();
    GetHiddenContainer()->AddComponent<UILayoutIgnorer>();
    GetHiddenContainer()->SetVisible(false);

    p_headersBar->SetParent(this);
    // GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Flexible,
    //                                       1, 1.0f)->SetParent(this);
    botPart->SetParent(this);
    GetHiddenContainer()->SetParent(botPart); // Behind
    // hiddenTabsFocusBlocker->SetParent(botPart); // Between
    GetVisibleContainer()->SetParent(botPart); // Front
}

UITabContainer::~UITabContainer()
{
}

void UITabContainer::AddTab(const String &title,
                            GameObject *tabbedChild,
                            uint index)
{
    if (!GetTabbedChildren().Contains(tabbedChild))
    {
        UITabHeader *tabHeader = GameObject::Create<UITabHeader>();
        tabHeader->SetTitle(title);
        tabHeader->SetTabbedChild(tabbedChild);
        AddTabByTabHeader(tabHeader, index);
    }
}

void UITabContainer::AddTabByTabHeader(UITabHeader *tabHeader,
                                       uint index_)
{
    if (!GetTabHeaders().Contains(tabHeader))
    {
        uint index = Math::Min(index_, GetTabbedChildren().Size());

        GameObject *prevCurrentTabbedChild = GetCurrentTabChild();

        GameObject *tabbedChild = tabHeader->GetTabbedChild();
        tabHeader->SetParent(p_headersBar, index);
        tabbedChild->SetParent(GetHiddenContainer());

        p_tabbedChildren.Insert(tabbedChild, index);
        m_childrenToHeader.Add(tabbedChild, tabHeader);

        tabHeader->SetTabContainer(this);
        tabHeader->EventEmitter<IEventsTabHeader>::RegisterListener(this);

        if (!GetCurrentTabChild())
        {
            SetCurrentTabChild(tabbedChild);
        }
        else
        {
            // This will update current index if needed because of the insertion
            SetCurrentTabChild(prevCurrentTabbedChild);
        }
    }
}

void UITabContainer::RemoveTab(GameObject *tabbedChild,
                               bool destroy)
{
    if (UITabHeader *tabHeader = GetTabHeaderFromChild(tabbedChild))
    {
        RemoveTabByHeader(tabHeader, destroy);
    }
}

void UITabContainer::RemoveTabByHeader(UITabHeader *tabHeader,
                                       bool destroy)
{
    if (GameObject *tabbedChild = tabHeader->GetTabbedChild())
    {
        uint removedTabIndex = p_tabbedChildren.IndexOf(tabbedChild);
        if (removedTabIndex == GetCurrentTabIndex())
        {
            SetCurrentTabIndex(-1u);
        }

        tabHeader->EventEmitter<IEventsTabHeader>::UnRegisterListener(this);
        tabHeader->SetParent(nullptr);
        tabbedChild->SetParent(nullptr);
        if (destroy)
        {
            GameObject::Destroy(tabbedChild);
            GameObject::Destroy(tabHeader);
        }
        p_tabbedChildren.Remove(tabbedChild);
        m_childrenToHeader.Remove(tabbedChild);

        // In case indices are changed, reselect current tab child
        if (GetTabbedChildren().Size() >= 1)
        {
            if (GetCurrentTabChild())
            {
                SetCurrentTabChild( GetCurrentTabChild() );
            }
            else
            {
                SetCurrentTabChild( GetTabbedChildren().Front() );
            }
        }
    }
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
    if (GetCurrentTabIndex() < GetTabbedChildren().Size())
    {
        auto it = p_tabbedChildren.Begin();
        std::advance(it, GetCurrentTabIndex());
        currentTabChild = *it;
    }
    return currentTabChild;
}

Array<UITabHeader *> UITabContainer::GetTabHeaders() const
{
    Array<UITabHeader*> tabHeaders;
    Array<GameObject*> tabChildren = GetTabbedChildren();
    for (GameObject *tabChild : tabChildren)
    {
        tabHeaders.PushBack( GetTabHeaderFromChild(tabChild) );
    }
    return tabHeaders;
}

const Array<GameObject *> &UITabContainer::GetTabbedChildren() const
{
    return p_tabbedChildren;
}

uint UITabContainer::GetCurrentTabIndex() const
{
    return m_currentTabIndex;
}

GameObject *UITabContainer::GetHeadersBar() const
{
    return p_headersBar;
}

void UITabContainer::SetCurrentTabIndex(int index)
{
    if (index != GetCurrentTabIndex())
    {
        if (index == -1u || index < GetTabbedChildren().Size())
        {
            m_currentTabIndex = index;
        }

        // Hide all tabs
        for (int i = 0; i < GetTabbedChildren().Size(); ++i)
        {
            GameObject *tabbedChild = GetTabbedChildren()[i];
            UITabHeader *header = GetTabHeaderFromChild(tabbedChild);
            header->SetInForeground( false );
            tabbedChild->SetVisible( false );
            tabbedChild->SetParent( GetHiddenContainer() );
        }

        // Show chosen current tab
        if (index < GetTabbedChildren().Size())
        {
            GameObject *tabbedChild = GetTabbedChildren()[index];
            UITabHeader *header = GetTabHeaderFromChild(tabbedChild);
            header->SetInForeground(true);
            tabbedChild->SetVisible(true);
            tabbedChild->SetParent( GetVisibleContainer() );
        }
    }
}

void UITabContainer::SetCurrentTabChild(GameObject *currentTabChild)
{
    uint index = GetTabbedChildren().IndexOf(currentTabChild);
    SetCurrentTabIndex(index);
}

void UITabContainer::OnTabHeaderClicked(UITabHeader *header)
{
    if (GameObject *tabbedChild = header->GetTabbedChild())
    {
        SetCurrentTabChild(tabbedChild);
    }

    EventEmitter<IEventsTabHeader>::PropagateToListeners(
                &IEventsTabHeader::OnTabHeaderClicked, header);
}

GameObject *UITabContainer::GetHiddenContainer() const
{
    return p_hiddenTabsContainer;
}

GameObject *UITabContainer::GetVisibleContainer() const
{
    return p_currentTabContainer;
}

UITabHeader *UITabContainer::GetTabHeaderFromChild(GameObject *tabbedChild) const
{
    auto it = m_childrenToHeader.Find(tabbedChild);
    if (it != m_childrenToHeader.End())
    {
        return it->second;
    }
    return nullptr;
}
