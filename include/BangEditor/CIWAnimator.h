#ifndef CIWANIMATOR_H
#define CIWANIMATOR_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class Animator;
class IEventsValueChanged;
class UICheckBox;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile;

class CIWAnimator : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWAnimator);

public:
    CIWAnimator();

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UICheckBox *p_playOnStartInput = nullptr;
    UIInputFile *p_animatorSMInput = nullptr;

    virtual ~CIWAnimator() override;

    UIInputFile *CreateAnimationEntry();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    Animator *GetAnimator() const;
};
}

#endif  // CIWANIMATOR_H
