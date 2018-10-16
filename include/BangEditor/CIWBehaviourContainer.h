#ifndef CIWBEHAVIOURCONTAINER_H
#define CIWBEHAVIOURCONTAINER_H

#include "Bang/BPReflectedVariable.h"
#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWBehaviour.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class BehaviourContainer;
FORWARD   class BPReflectedStruct;
FORWARD   class IEventsValueChanged;
FORWARD   class UIInputFileWithPreview;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFileWithPreview;

class CIWBehaviourContainer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviourContainer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    BehaviourContainer *GetBehaviourContainer() const;

private:
    UIInputFileWithPreview *p_sourceInputFile = nullptr;

    CIWBehaviourHelper m_ciwBehaviourHelper;
    Time m_prevTimeHeaderChanged;

    CIWBehaviourContainer();
    virtual ~CIWBehaviourContainer() override;

    void UpdateFromReflection(const BPReflectedStruct &reflectStruct);
    void UpdateInitializationMetaFromWidgets();

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWBEHAVIOURCONTAINER_H

