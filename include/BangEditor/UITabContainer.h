#ifndef UITABCONTAINER_H
#define UITABCONTAINER_H

#include "Bang/List.h"
#include "Bang/UMap.h"
#include "Bang/GameObject.h"

#include "BangEditor/UITabHeader.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabHeader;

class UITabContainer : public GameObject,
                       public ITabHeaderListener
{
    GAMEOBJECT_EDITOR(UITabContainer);

public:
    void AddTab(const String &title, GameObject *tabbedChild);
    void RemoveTab(GameObject *tabbedChild);

    void SetCurrentTabIndex(int index);
    void SetCurrentTabChild(GameObject *currentTabChild);
    void SetTabTitle(GameObject *tabbedChild, const String &title);

    int GetCurrentTabIndex() const;
    GameObject* GetCurrentTabChild() const;

    const List<GameObject*>& GetChildrenInTabs() const;

private:
    int m_currentTabIndex = -1;
    List<GameObject*> p_childrenInTabs;
    UMap<GameObject*, UITabHeader*> m_childrenToHeader;
    UMap<UITabHeader*, GameObject*> m_headerToChildren;

    GameObject *p_headersBar = nullptr;
    GameObject *p_hiddenTabsContainer = nullptr;
    GameObject *p_currentTabContainer = nullptr;

    // ITabHeaderListener
    void OnTabHeaderClicked(UITabHeader *header) override;

    GameObject* GetHiddenTabsContainer() const;
    GameObject* GetCurrentTabContainer() const;

	UITabContainer();
	virtual ~UITabContainer();
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABCONTAINER_H

