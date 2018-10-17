#ifndef CIWBEHAVIOURCONTAINER_H
#define CIWBEHAVIOURCONTAINER_H

#include "Bang/BPReflectedVariable.h"
#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class BehaviourContainer;
class BPReflectedStruct;
class IEventsValueChanged;
class UIInputFileWithPreview;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileWithPreview;

class CIWBehaviourContainer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviourContainer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    BehaviourContainer *GetBehaviourContainer() const;

private:
    mutable BPReflectedStruct m_behaviourReflectStruct;
    mutable Time m_prevTimeHeaderChanged;

    UIInputFileWithPreview *p_sourceInputFile = nullptr;

    CIWBehaviourContainer();
    virtual ~CIWBehaviourContainer() override;

    void UpdateFromReflection(const BPReflectedStruct &reflectStruct);
    void UpdateInitializationMetaFromWidgets();

    // ComponentInspectorWidget
    void OnComponentSet() override;
    BPReflectedStruct GetComponentReflectStruct() const override;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWBEHAVIOURCONTAINER_H
