#ifndef CIWBEHAVIOURCONTAINER_H
#define CIWBEHAVIOURCONTAINER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/ReflectVariable.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class BehaviourContainer;
class ReflectStruct;
class IEventsValueChanged;
template <class>
class EventEmitter;
class UIButton;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile;

class CIWBehaviourContainer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(CIWBehaviourContainer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    BehaviourContainer *GetBehaviourContainer() const;

private:
    UIInputFile *p_sourceInputFile = nullptr;
    UIButton *p_resetValuesButton = nullptr;

    CIWBehaviourContainer();
    virtual ~CIWBehaviourContainer() override;

    void UpdateFromReflection(const ReflectStruct &reflectStruct);
    void UpdateModifiedInitializationMetaFromWidget(GameObject *widget);

    // ComponentInspectorWidget
    void OnComponentSet() override;
    ReflectStruct GetReflectableReflectStruct() const override;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWBEHAVIOURCONTAINER_H
