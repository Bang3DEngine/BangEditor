#ifndef CIWBEHAVIOUR_H
#define CIWBEHAVIOUR_H

#include "Bang/Time.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/BPReflectedVariable.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Behaviour;
FORWARD class UIInputFileWithPreview;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFileWithPreview;

class CIWBehaviourHelper
{
public:
    void RecreateWidgetsFromReflection(const BPReflectedStruct &reflectStruct,
                                       EventListener<IEventsValueChanged> *listener);
    void UpdateWidgetsFromMeta(const MetaNode &meta);

    MetaNode GetMetaFromWidgets() const;

    const Map<GameObject*, BPReflectedVariable>& GetWidgetToReflectedVar() const;
    const Map<String, GameObject*>& GetVarNameToWidget() const;
    const Array<GameObject*>& GetWidgets() const;

private:
    Map<GameObject*, BPReflectedVariable> m_widgetToReflectedVar;
    Map<String, GameObject*> m_varNameToWidget;
    Array<GameObject*> m_widgets;
};

class CIWBehaviour : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviour);

public:
	CIWBehaviour();
	virtual ~CIWBehaviour();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    Behaviour *GetBehaviour() const;
    void OnComponentSet() override;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    CIWBehaviourHelper m_ciwBehaviourHelper;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWBEHAVIOUR_H

