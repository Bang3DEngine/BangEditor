#ifndef REFLECTWIDGETSMANAGER_H
#define REFLECTWIDGETSMANAGER_H

#include "Bang/Bang.h"
#include "Bang/Map.h"
#include "Bang/ReflectStruct.h"
#include "Bang/ReflectVariable.h"

#include "BangEditor/BangEditor.h"

namespace Bang
{
class GameObject;
class IEventsValueChanged;
template <class>
class EventListener;
};

using namespace Bang;
namespace BangEditor
{
class InspectorWidget;

class ReflectWidgetsManager
{
public:
    void UpdateWidgetsFromReflection(const ReflectStruct &reflectStruct,
                                     InspectorWidget *inspectorWidget);
    void UpdateWidgetsContentFromMeta(const MetaNode &meta);

    MetaNode GetMetaFromWidgets() const;

    const Map<GameObject *, ReflectVariable> &GetWidgetToReflectedVar() const;
    const Map<String, GameObject *> &GetVarNameToWidget() const;
    const Array<GameObject *> &GetWidgets() const;

private:
    ReflectStruct m_previousReflectedStruct;
    Map<GameObject *, ReflectVariable> m_widgetToReflectedVar;
    Map<String, GameObject *> m_varNameToWidget;
    Array<GameObject *> m_widgets;
};
}

#endif  // REFLECTWIDGETSMANAGER_H
