#ifndef UITABCONTAINER_H
#define UITABCONTAINER_H

#include <sys/types.h>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/UMap.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UITabHeader.h"

namespace BangEditor {
class IEventsTabHeader;
}  // namespace BangEditor

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabHeader;

class UITabContainer : public GameObject,
                       public EventEmitter<IEventsTabHeader>,
                       public EventListener<IEventsTabHeader>
{
    GAMEOBJECT_EDITOR(UITabContainer);

public:
    void AddTab(const String &title, GameObject *tabbedChild, uint index = -1u);
    void AddTabByTabHeader(UITabHeader *tabHeader, uint index = -1u);
    void RemoveTab(GameObject *tabbedChild, bool destroy = true);
    void RemoveTabByHeader(UITabHeader *tabHeader, bool destroy = true);

    void SetCurrentTabIndex(int index);
    void SetCurrentTabChild(GameObject *currentTabChild);
    void SetTabTitle(GameObject *tabbedChild, const String &title);

    uint GetCurrentTabIndex() const;
    GameObject* GetHeadersBar() const;
    GameObject* GetCurrentTabChild() const;

    Array<UITabHeader*> GetTabHeaders() const;
    const Array<GameObject*>& GetTabbedChildren() const;

private:
    uint m_currentTabIndex = -1u;
    Array<GameObject*> p_tabbedChildren;
    UMap<GameObject*, UITabHeader*> m_childrenToHeader;

    GameObject *p_headersBar = nullptr;
    GameObject *p_hiddenTabsContainer = nullptr;
    GameObject *p_currentTabContainer = nullptr;

    // IEventsTabHeader
    void OnTabHeaderClicked(UITabHeader *header) override;

    GameObject* GetHiddenContainer() const;
    GameObject* GetVisibleContainer() const;
    UITabHeader* GetTabHeaderFromChild(GameObject *tabbedChild) const;
    GameObject* GetChildFromTabHeader(UITabHeader *tabHeader) const;

	UITabContainer();
	virtual ~UITabContainer();
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABCONTAINER_H

