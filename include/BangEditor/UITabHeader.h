#ifndef UITABHEADER_H
#define UITABHEADER_H

#include "Bang/Bang.h"
#include "Bang/DPtr.h"
#include "Bang/String.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsDragDrop.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsTabHeader.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class UITextRenderer;
FORWARD class UIDragDroppable;
FORWARD class UIImageRenderer;
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
    const Color ForegroundColor = Color::Zero;
    const Color BackgroundColor = Color::DarkGray;

    String m_title = "Tab";
    bool m_inForeground = false;
    DPtr<GameObject> p_tabbedChild = nullptr;
    Color m_currentHeaderColor = Color::Zero;
    DPtr<UITabContainer> p_tabContainer = nullptr;

    UIImageRenderer *p_bg = nullptr;
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

