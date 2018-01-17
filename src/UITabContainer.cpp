#include "BangEditor/UITabContainer.h"

#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
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

    p_headersBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *headerBarHL = p_headersBar->AddComponent<UIHorizontalLayout>();
    headerBarHL->SetChildrenVerticalStretch(Stretch::None);
    headerBarHL->SetChildrenVerticalAlignment(VerticalAlignment::Bot);

    UILayoutElement *headerBarLE = p_headersBar->AddComponent<UILayoutElement>();
    headerBarLE->SetMinHeight(15);
    headerBarLE->SetLayoutPriority(2);

    p_currentTabContainer = GameObjectFactory::CreateUIGameObject();

    UILayoutElement *containerLE =
                    p_currentTabContainer->AddComponent<UILayoutElement>();
    containerLE->SetFlexibleSize(Vector2::One);

    p_currentTabContainer->AddComponent<UIHorizontalLayout>();

    p_headersBar->SetParent(this);
    // GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Flexible,
    //                                       1, 1.0f)->SetParent(this);
    p_currentTabContainer->SetParent(this);
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

        tabHeader->EventEmitter<ITabHeaderListener>::RegisterListener(this);

        tabbedChild->SetEnabled(false);
        if (!GetCurrentTabChild()) { SetCurrentTabChild(tabbedChild); }
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
        UITabHeader *headerGo = m_childrenToHeader.Get(tabbedChild);
        UITextRenderer *titleText = headerGo->GetComponent<UITextRenderer>();
        titleText->SetContent(title);
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

            header->SetInForeground( i == index );
            tabbedChild->SetEnabled( i == index );

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
}

GameObject *UITabContainer::GetCurrentTabContainer() const
{
    return p_currentTabContainer;
}

