#ifndef COMPONENTINSPECTORWIDGET_H
#define COMPONENTINSPECTORWIDGET_H

#include "Bang/Bang.h"
#include "Bang/MetaNode.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/UIContextMenu.h"
#include "BangEditor/InspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UICheckBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ComponentInspectorWidget : public InspectorWidget,
                                 public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(InspectorWidget);

protected:
    ComponentInspectorWidget();
    virtual ~ComponentInspectorWidget();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    virtual void SetComponent(Component *comp);

    void PushCurrentStateToUndoRedo(const MetaNode &undoMetaBefore);
    void PushCurrentStateToUndoRedoIfAnyChangeForGameObject(
                                                const MetaNode &undoMetaBefore);
    Component *GetComponent() const;
    GameObject *GetInspectedGameObject() const;

    virtual void SetTitle(const String& title) override;

    virtual bool CanBeRemovedFromContextMenu() const;
    virtual Texture2D* GetComponentIconTexture() const;
    virtual Color GetComponentIconTint() const;
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object);

private:
    Component *p_component = nullptr;
    UIContextMenu *p_contextMenu = nullptr;

    // UIContextMenu
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    void MoveComponent(Component *comp, int offset);

    virtual void OnComponentSet();
    virtual bool MustShowEnabledCheckbox() const;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTINSPECTORWIDGET_H

