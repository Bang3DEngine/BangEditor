#ifndef CIWBEHAVIOURCONTAINER_H
#define CIWBEHAVIOURCONTAINER_H

#include "Bang/IValueChangedListener.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputFile;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;
FORWARD class BehaviourContainer;

class CIWBehaviourContainer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviourContainer);

public:
    // InspectorWidget
    virtual void Init() override;

    // ComponentInspectorWidget
    void UpdateValuesFromComponent() override;

    BehaviourContainer *GetBehaviourContainer() const;

private:
    UIInputFile *p_sourceInputFile = nullptr;

    CIWBehaviourContainer();
    virtual ~CIWBehaviourContainer();

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWBEHAVIOURCONTAINER_H

