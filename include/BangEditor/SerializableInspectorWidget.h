#ifndef SERIALIZABLEINSPECTORWIDGET_H
#define SERIALIZABLEINSPECTORWIDGET_H

#include "Bang/Bang.h"
#include "Bang/Map.h"
#include "Bang/ReflectStruct.h"
#include "Bang/ReflectVariable.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

namespace Bang
{
class IReflectable;
class Serializable;
}

using namespace Bang;
namespace BangEditor
{
class SerializableInspectorWidget : public InspectorWidget
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(SerializableInspectorWidget);

public:
    SerializableInspectorWidget();
    virtual ~SerializableInspectorWidget() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    virtual void OnReflectableSet();
    virtual ReflectStruct GetReflectableReflectStruct() const;

    void SetSerializable(Serializable *serializable);

    void UpdateReflectWidgetsFromReflection(const ReflectStruct &reflectStruct,
                                            InspectorWidget *inspectorWidget);
    void UpdateWidgetsContentFromMeta(const MetaNode &meta);

    bool NeedsToRecreateWidget(const ReflectVariable &previousVariable,
                               const ReflectVariable &currentVariable) const;
    MetaNode GetMetaFromWidget(GameObject *widget) const;
    MetaNode GetMetaFromReflectWidgets() const;

    Serializable *GetSerializable() const;
    const Map<GameObject *, ReflectVariable> &GetWidgetToReflectedVar() const;
    const Map<String, GameObject *> &GetVarNameToWidget() const;
    const Array<GameObject *> &GetWidgets() const;

protected:
    // IEventsValueChanged
    virtual void OnValueChanged(
        EventEmitter<IEventsValueChanged> *object) override;

private:
    Serializable *p_serializable = nullptr;

    ReflectStruct m_previousReflectStruct;
    Map<GameObject *, ReflectVariable> m_reflectWidgetToReflectVar;
    Map<String, GameObject *> m_varNameToReflectWidget;
    Array<GameObject *> m_reflectWidgets;
};
}

#endif  // SERIALIZABLEINSPECTORWIDGET_H
