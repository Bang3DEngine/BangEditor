#ifndef GAMEOBJECTINSPECTORWIDGET_H
#define GAMEOBJECTINSPECTORWIDGET_H

#include "Bang/Bang.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/InspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputText;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class GameObjectInspectorWidget : public InspectorWidget,
                                  public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(GameObjectInspectorWidget);

public:
	GameObjectInspectorWidget();
	virtual ~GameObjectInspectorWidget();

    void SetGameObject(GameObject *gameObject);
    GameObject *GetGameObject() const;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    GameObject *p_gameObject = nullptr;

    UIInputText *p_nameInput = nullptr;
    UICheckBox *p_enabledInput = nullptr;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // GAMEOBJECTINSPECTORWIDGET_H

