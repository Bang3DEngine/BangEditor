#ifndef CIWBOXCOLLIDER_H
#define CIWBOXCOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

namespace BangEditor {
class UIInputVector;
}  // namespace BangEditor
namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class BoxCollider;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWBoxCollider : public CIWCollider
{
    GAMEOBJECT_EDITOR(CIWBoxCollider);

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputVector *p_extentsInput = nullptr;

    CIWBoxCollider();
    virtual ~CIWBoxCollider();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    BoxCollider *GetBoxCollider() const;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWBOXCOLLIDER_H

