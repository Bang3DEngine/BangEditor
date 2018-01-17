#ifndef UITABCONTAINER_H
#define UITABCONTAINER_H

#include "Bang/List.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UITabContainer : public GameObject
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
    Map<GameObject*, GameObject*> m_childrenToTitleGo;

    GameObject *p_titleBar = nullptr;
    GameObject *p_currentTabContainer = nullptr;

    GameObject* GetCurrentTabContainer() const;

	UITabContainer();
	virtual ~UITabContainer();
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABCONTAINER_H

