#ifndef CIWMESHCOLLIDER_H
#define CIWMESHCOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshCollider;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

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
	virtual ~CIWMeshCollider();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    MeshCollider *GetMeshCollider() const;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWMESHCOLLIDER_H

