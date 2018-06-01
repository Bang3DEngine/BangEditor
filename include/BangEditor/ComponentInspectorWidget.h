#ifndef COMPONENTINSPECTORWIDGET_H
#define COMPONENTINSPECTORWIDGET_H

#include "Bang/Bang.h"
#include "Bang/XMLNode.h"
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
                                 public EventListener<IEventsValueChanged>,
                                 public EventListener<IEventsFocus>,
                                 public EventListener<IEventsComponent>
{
    GAMEOBJECT_EDITOR(InspectorWidget);

protected:
    ComponentInspectorWidget();
    virtual ~ComponentInspectorWidget();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    virtual void SetComponent(Component *comp);

protected:
    Component *GetComponent() const;
    GameObject *GetInspectedGameObject() const;

    virtual void SetTitle(const String& title) override;
    virtual GameObject *CreateTitleGameObject() override;

    virtual bool CanBeRemovedFromContextMenu() const;

    virtual RH<Texture2D> GetComponentIconTexture() const;

    void PushCurrentStateToUndoRedoIfAnyChange();

    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object);

private:
    Component *p_component = nullptr;

    XMLNode m_undoXMLBefore;
    UIContextMenu *p_contextMenu = nullptr;

    UIImageRenderer *p_icon = nullptr;
    UITextRenderer *p_titleText = nullptr;
    UICheckBox *p_enabledCheckBox = nullptr;

    // UIContextMenu
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    void MoveComponent(Component *comp, int offset);

    virtual bool MustShowEnabledCheckbox() const;
    void UpdateTrackChildrenFocusEmitters(GameObject *child,
                                          bool track,
                                          bool recursive);

    // IEventsChildren
    void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;

    // IEventsComponent
    void OnComponentAdded(Component *addedComponent, int index) override;
    void OnComponentRemoved(Component *removedComponent) override;

    // IEventsFocus
    void OnFocusTaken(EventEmitter<IEventsFocus> *focusEmitter) override;
    void OnFocusLost(EventEmitter<IEventsFocus> *focusEmitter) override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTINSPECTORWIDGET_H

