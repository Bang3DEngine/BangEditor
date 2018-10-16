#ifndef CIWANIMATOR_H
#define CIWANIMATOR_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class Animator;
FORWARD   class IEventsValueChanged;
FORWARD   class UICheckBox;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;
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
    UIInputFileWithPreview *p_animatorSMInput = nullptr;

    CIWAnimator();
    virtual ~CIWAnimator() override;

    UIInputFileWithPreview* CreateAnimationEntry();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    Animator *GetAnimator() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWANIMATOR_H

