#ifndef UITABHEADER_H
#define UITABHEADER_H

#include <sys/types.h>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/DPtr.h"
#include "Bang/DPtr.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsTabHeader.h"

namespace BangEditor {
class IEventsTabHeader;
}  // namespace BangEditor
namespace Bang {
class IEventsDragDrop;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIDragDroppable;
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD class UITextRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabContainer;

class UITabHeader : public GameObject,
                    public EventEmitter<IEventsTabHeader>,
                    public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR(UITabHeader);

public:
	UITabHeader();
	virtual ~UITabHeader();

    // GameObject
    void Update() override;

    void SetTitle(const String &title);
    void SetInForeground(bool inForeground);

    const String &GetTitle() const;
    GameObject *GetTabbedChild() const;
    UITabContainer *GetTabContainer() const;
    UIDragDroppable *GetDragDroppable() const;

private:
    static const Color ForegroundColor;
    static const Color BackgroundColor;

    String m_title = "Tab";
    bool m_inForeground = false;
    DPtr<GameObject> p_tabbedChild = nullptr;
    Color m_currentHeaderColor = Color::Zero;
    DPtr<UITabContainer> p_tabContainer = nullptr;

    UIImageRenderer *p_bg = nullptr;
    UIImageRenderer *p_border = nullptr;
    UIFocusable *p_focusable = nullptr;
    UITextRenderer *p_titleText = nullptr;

    uint m_dragTabContainerDestinyIndex = -1;
    DPtr<UITabContainer> p_lastTabContainer = nullptr;
    DPtr<UITabContainer> p_dragTabContainerDestiny = nullptr;
    UIDragDroppable *p_dragDroppable = nullptr;

    void SetTabContainer(UITabContainer *tabContainer);
    void SetTabbedChild(GameObject *tabbedChild);

    // IEventsDragDrop
    virtual void OnDragStarted(EventEmitter<IEventsDragDrop>
                               *dragDropEmitter) override;
    virtual void OnDragUpdate(EventEmitter<IEventsDragDrop>
                              *dragDropEmitter) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDropEmitter) override;

    friend class UITabStation;
    friend class UITabContainer;
};

NAMESPACE_BANG_EDITOR_END

#endif // UITABHEADER_H

