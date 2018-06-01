#ifndef CIWBEHAVIOURCONTAINER_H
#define CIWBEHAVIOURCONTAINER_H

#include "Bang/IEventsValueChanged.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputFile;
FORWARD class BehaviourContainer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

class CIWBehaviourContainer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviourContainer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    BehaviourContainer *GetBehaviourContainer() const;

private:
    UIInputFile *p_sourceInputFile = nullptr;

    CIWBehaviourContainer();
    virtual ~CIWBehaviourContainer();

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWBEHAVIOURCONTAINER_H

