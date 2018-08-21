#ifndef CIWSPHERECOLLIDER_H
#define CIWSPHERECOLLIDER_H

#include "Bang/Bang.h"

#include "BangEditor/CIWCollider.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class SphereCollider;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWSphereCollider : public CIWCollider
{
    GAMEOBJECT_EDITOR(CIWSphereCollider);

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_radiusInput = nullptr;

    CIWSphereCollider();
    virtual ~CIWSphereCollider();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    SphereCollider *GetSphereCollider() const;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWSPHERECOLLIDER_H

