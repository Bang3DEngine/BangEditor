#ifndef GAMEOBJECTINSPECTORWIDGET_H
#define GAMEOBJECTINSPECTORWIDGET_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

namespace Bang
{
class UIInputText;
class GameObject;
class IEventsValueChanged;
class UICheckBox;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class GameObjectInspectorWidget : public InspectorWidget,
                                  public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(GameObjectInspectorWidget);

public:
    GameObjectInspectorWidget();
    virtual ~GameObjectInspectorWidget() override;

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
    virtual void OnValueChanged(
        EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // GAMEOBJECTINSPECTORWIDGET_H
