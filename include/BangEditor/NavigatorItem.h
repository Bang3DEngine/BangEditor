#ifndef NAVIGATORITEM_H
#define NAVIGATORITEM_H

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
#include "Bang/IEventsFocus.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "BangEditor/BangEditor.h"
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

class NavigatorItem : public GameObject, public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(NavigatorItem);

public:
    NavigatorItem();

    void SetSelected(bool selected);

    bool IsSelected() const;
    UILabel *GetLabel() const;
    UIImageRenderer *GetIcon() const;
    UIAspectRatioFitter *GetAspectRatioFitter() const;
    UIFocusable *GetFocusable() const;

protected:
    virtual ~NavigatorItem() override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

private:
    bool m_selected = false;

    DPtr<UILabel> p_label;
    DPtr<UIImageRenderer> p_bg;
    DPtr<UIImageRenderer> p_icon;
    DPtr<UIFocusable> p_focusable;
    DPtr<UIAspectRatioFitter> p_aspectRatioFitter;

    virtual void OnFocusTaken(UIFocusable *focusable, const UIEvent &event);
};
}  // namespace BangEditor

#endif  // NAVIGATORITEM_H
