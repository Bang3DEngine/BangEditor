#ifndef RIWANIMATORSTATEMACHINE_H
#define RIWANIMATORSTATEMACHINE_H

#include <vector>

#include "Bang/AnimatorStateMachine.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

namespace Bang
{
class AnimatorStateMachine;
class IEventsValueChanged;
class Texture2D;
class UIButton;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class RIWAnimatorStateMachine : public RIWResource<AnimatorStateMachine>
{
    GAMEOBJECT_EDITOR(RIWAnimatorStateMachine);

public:
    // InspectorWidget
    void Init() override;

private:
    UIButton *p_openInEditorButton = nullptr;

    RIWAnimatorStateMachine();
    virtual ~RIWAnimatorStateMachine() override;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedRIWResource(
        EventEmitter<IEventsValueChanged> *object) override;

    AnimatorStateMachine *GetAnimatorStateMachine() const;
};
}

#endif  // RIWANIMATORSTATEMACHINE_H
