#ifndef CIWTRANSFORM_H
#define CIWTRANSFORM_H

#include "Bang/IValueChangedListener.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Transform;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputVector;

class CIWTransform : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWTransform);

public:

    // InspectorWidget
    void Init() override;

    void Update() override;

private:
    Transform *p_relatedTransform = nullptr;

    UIInputVector *p_posIV   = nullptr;
    UIInputVector *p_rotIV   = nullptr;
    UIInputVector *p_scaleIV = nullptr;

    CIWTransform();
    virtual ~CIWTransform();


    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    // ComponentWidget
    void SetComponent(Component *comp) override;
    bool MustShowEnabledCheckbox() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWTRANSFORM_H

