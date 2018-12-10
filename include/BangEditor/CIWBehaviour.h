#ifndef CIWBEHAVIOUR_H
#define CIWBEHAVIOUR_H

#include <functional>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Map.h"
#include "Bang/Map.tcc"
#include "Bang/MetaNode.h"
#include "Bang/ReflectVariable.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class Behaviour;
class UIInputFile;
class ReflectStruct;
class GameObject;
class IEventsValueChanged;
template <class>
class EventEmitter;
template <class>
class EventListener;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile;

class CIWBehaviour : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWBehaviour);

public:
    CIWBehaviour();
    virtual ~CIWBehaviour() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    Behaviour *GetBehaviour() const;
};
}

#endif  // CIWBEHAVIOUR_H
