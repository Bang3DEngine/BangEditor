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
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    CIWTransform() = default;
    virtual ~CIWTransform() = default;

    virtual bool ShowRemoveInMenu() const override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

private:
    UIInputVector *p_posIV   = nullptr;
    UIInputVector *p_rotIV   = nullptr;
    UIInputVector *p_scaleIV = nullptr;

    Transform *GetTransform() const;

    // ComponentWidget
    bool MustShowEnabledCheckbox() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWTRANSFORM_H

