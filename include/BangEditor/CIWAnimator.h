#ifndef CIWANIMATOR_H
#define CIWANIMATOR_H

#include "Bang/Bang.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Animator;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputArray;
FORWARD class UIInputFileWithPreview;

class CIWAnimator : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWAnimator);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UICheckBox *p_playOnStartInput = nullptr;
    UIInputArray *p_animationsInput = nullptr;

    CIWAnimator();
    virtual ~CIWAnimator();

    UIInputFileWithPreview* CreateAnimationEntry();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    Animator *GetAnimator() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWANIMATOR_H

