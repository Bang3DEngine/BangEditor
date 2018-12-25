#ifndef COMPONENTINSPECTORWIDGET_H
#define COMPONENTINSPECTORWIDGET_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/MetaNode.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SerializableInspectorWidget.h"
#include "BangEditor/UIContextMenu.h"

namespace Bang
{
class Component;
class GameObject;
class IEventsValueChanged;
class Texture2D;
class UICheckBox;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class MenuItem;
class UIContextMenu;

class ComponentInspectorWidget : public SerializableInspectorWidget
{
    GAMEOBJECT_EDITOR(ComponentInspectorWidget);

protected:
    ComponentInspectorWidget();
    virtual ~ComponentInspectorWidget() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    virtual void SetComponent(Component *comp);

    void PushCurrentStateToUndoRedo(const MetaNode &undoMetaBefore);
    void PushCurrentStateToUndoRedoIfAnyChangeForGameObject(
        const MetaNode &undoMetaBefore);
    Component *GetComponent() const;
    GameObject *GetInspectedGameObject() const;

    virtual void SetTitle(const String &title) override;

    virtual bool CanBeRemovedFromContextMenu() const;
    virtual Texture2D *GetComponentIconTexture() const;
    virtual Color GetComponentIconTint() const;
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object);

    virtual void OnComponentSet();
    virtual bool MustShowEnabledCheckbox() const;

    // ReflectableInspectorWidget
    void OnReflectableSet() override;

private:
    Component *p_component = nullptr;
    UIContextMenu *p_contextMenu = nullptr;

    // UIContextMenu
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    void MoveComponent(Component *comp, int offset);

    // IEventsValueChanged
    virtual void OnValueChanged(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // COMPONENTINSPECTORWIDGET_H
