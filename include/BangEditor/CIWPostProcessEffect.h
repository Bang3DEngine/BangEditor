#ifndef CIWPOSTPROCESSEFFECT_H
#define CIWPOSTPROCESSEFFECT_H

#include "Bang/Bang.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class PostProcessEffect;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

class CIWPostProcessEffect : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWPostProcessEffect);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_priorityInput = nullptr;
    UIInputFile *p_fragmentShaderInput = nullptr;
    UIComboBox *p_typeComboBox = nullptr;

    PostProcessEffect* GetPostProcessEffect() const;

    // IValueChangedListener
    virtual void OnValueChanged(Object *object) override;

    CIWPostProcessEffect() = default;
    virtual ~CIWPostProcessEffect() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWPOSTPROCESSEFFECT_H

