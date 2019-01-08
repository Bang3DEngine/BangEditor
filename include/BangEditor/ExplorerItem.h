#ifndef EXPLORERITEM_H
#define EXPLORERITEM_H

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
#include "BangEditor/IEventsExplorerItem.h"
#include "BangEditor/NavigatorItem.h"
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
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class IEventsExplorerItem;
class MenuItem;

class ExplorerItem : public NavigatorItem,
                     public EventListener<IEventsDragDrop>,
                     public EventEmitter<IEventsExplorerItem>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(ExplorerItem);

public:
    ExplorerItem();

    void SetPath(const Path &path);
    void SetPathString(const String &string);

    const Path &GetPath() const;
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
    virtual ~ExplorerItem() override;

private:
    Path m_path = Path::Empty();
    String m_pathString = "";

    DPtr<UIContextMenu> p_contextMenu;
    DPtr<UIDragDroppable> p_dragDroppable;

    void OnFocusTaken(UIFocusable *focusable, const UIEvent &event) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};
}  // namespace BangEditor

#endif  // EXPLORERITEM_H
