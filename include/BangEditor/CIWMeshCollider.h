#ifndef CIWMESHCOLLIDER_H
#define CIWMESHCOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

namespace Bang
{
class IEventsValueChanged;
class MeshCollider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile;

class CIWMeshCollider : public CIWCollider
{
    GAMEOBJECT_EDITOR(CIWMeshCollider);

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

public:
    UIInputFile *p_meshInput = nullptr;

    CIWMeshCollider();
    virtual ~CIWMeshCollider() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    MeshCollider *GetMeshCollider() const;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWMESHCOLLIDER_H
