#ifndef EXPLORERITEM_H
#define EXPLORERITEM_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/DPtr.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsFocus.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsExplorerItem.h"
#include "BangEditor/UIContextMenu.h"

namespace Bang
{
class IEventsDragDrop;
class IFocusable;
class UIAspectRatioFitter;
class UIDragDroppable;
class UIFocusable;
class UIImageRenderer;
class UILabel;
}

using namespace Bang;
namespace BangEditor
{
class IEventsExplorerItem;
class MenuItem;

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
    const Path &GetPath() const;
    UIFocusable *GetFocusable() const;
    const String &GetPathString() const;

    void Rename();
    void Remove();
    void Paste();
    void Duplicate();

    // UIContextMenu callback
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    // IEventsDragDrop
    void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable) override;

protected:
    ExplorerItem();
    virtual ~ExplorerItem() override;

private:
    bool m_selected = false;
    Path m_path = Path::Empty;
    String m_pathString = "";

    DPtr<UILabel> p_label;
    DPtr<UIImageRenderer> p_bg;
    DPtr<UIImageRenderer> p_icon;
    DPtr<UIFocusable> p_focusable;
    DPtr<UIContextMenu> p_contextMenu;
    DPtr<UIDragDroppable> p_dragDroppable;
    DPtr<UIAspectRatioFitter> p_aspectRatioFitter;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};
}

#endif  // EXPLORERITEM_H
