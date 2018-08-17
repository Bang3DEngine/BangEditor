#ifndef EXPLORERITEM_H
#define EXPLORERITEM_H

#include "Bang/Bang.h"
#include "Bang/Path.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsDragDrop.h"

#include "BangEditor/UIContextMenu.h"
#include "BangEditor/IEventsExplorerItem.h"

USING_NAMESPACE_BANG

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class IFocusable;
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD class UIDragDroppable;
FORWARD class UIAspectRatioFitter;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

class ExplorerItem : public GameObject,
                     public EventListener<IEventsFocus>,
                     public EventListener<IEventsDragDrop>,
                     public EventEmitter<IEventsExplorerItem>
{
    GAMEOBJECT_EDITOR(ExplorerItem);

public:
    void SetPath(const Path &path);
    void SetSelected(bool selected);
    void SetPathString(const String &string);

    bool IsSelected() const;
    UILabel *GetLabel() const;
    const Path& GetPath() const;
    UIFocusable* GetFocusable() const;
    const String& GetPathString() const;

    void Rename();
    void Remove();
    void Paste();
    void Duplicate();

    // UIContextMenu callback
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    // IEventsDragDrop
    void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable, bool inside) override;

protected:
    ExplorerItem();
    virtual ~ExplorerItem();

private:
    bool m_selected = false;
    Path m_path = Path::Empty;
    String m_pathString = "";

    UILabel *p_label = nullptr;
    UIImageRenderer *p_bg = nullptr;
    UIImageRenderer *p_icon = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UIDragDroppable *p_dragDroppable = nullptr;
    UIAspectRatioFitter *p_aspectRatioFitter = nullptr;

    // IEventsFocus
    virtual void OnEvent(IFocusable *focusable, const UIEvent &event) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EXPLORERITEM_H

