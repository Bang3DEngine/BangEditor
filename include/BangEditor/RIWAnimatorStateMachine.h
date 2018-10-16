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

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class AnimatorStateMachine;
FORWARD   class IEventsValueChanged;
FORWARD   class Texture2D;
FORWARD   class UIButton;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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

NAMESPACE_BANG_EDITOR_END

#endif // RIWANIMATORSTATEMACHINE_H

